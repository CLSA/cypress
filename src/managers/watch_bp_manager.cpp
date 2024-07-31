#include "watch_bp_manager.h"
#include "auxiliary/json_settings.h"

#include "data/blood_pressure/tests/watch_bp_test.h"

#include <QMessageBox>
#include <QJsonObject>
#include <QJsonArray>
#include <QSqlError>

#include <QFile>
#include <QDir>

#include <windows.h>
#include <TlHelp32.h>

WatchBPManager::WatchBPManager(QSharedPointer<WatchBPSession> session): ManagerBase(session)
{
    m_processName = CypressSettings::readSetting("watch_bp/processName").toString();
    m_runnableName = CypressSettings::readSetting("watch_bp/runnableName").toString();
    m_runnablePath = CypressSettings::readSetting("watch_bp/runnablePath").toString();
    m_databasePath = CypressSettings::readSetting("watch_bp/databasePath").toString();
    m_backupDatabasePath = CypressSettings::readSetting("watch_bp/backupDatabasePath").toString();

    m_test.reset(new WatchBPTest);
}

bool WatchBPManager::isInstalled()
{
    const QString processName = CypressSettings::readSetting("watch_bp/processName").toString();
    const QString runnableName = CypressSettings::readSetting("watch_bp/runnableName").toString();
    const QString runnablePath = CypressSettings::readSetting("watch_bp/runnablePath").toString();
    const QString databasePath = CypressSettings::readSetting("watch_bp/databasePath").toString();
    const QString backupDatabasePath = CypressSettings::readSetting("watch_bp/backupDatabasePath").toString();

    if (backupDatabasePath.isEmpty() || backupDatabasePath.isNull()) {
        qInfo() << "WatchBPManager::isInstalled - backupDatabasePath is not defined";
        return false;
    }

    if (processName.isEmpty() || processName.isNull()) {
        qInfo() << "WatchBPManager::isInstalled - processName is not defined";
        return false;
    }

    if (runnableName.isEmpty() || runnableName.isNull()) {
        qInfo() << "WatchBPManager::isInstalled - runnableName is not defined";
        return false;
    }

    if (runnablePath.isEmpty() || runnablePath.isNull()) {
        qInfo() << "WatchBPManager::isInstalled - runnablePath is not defined";
        return false;
    }

    if (databasePath.isEmpty() || databasePath.isNull()) {
        qInfo() << "WatchBPManager::isInstalled - databasePath is not defined";
        return false;
    }

    const QDir runnablePathInfo(runnablePath);
    const QFileInfo runnableNameInfo(runnableName);
    const QFileInfo backupDatabasePathInfo(backupDatabasePath);

    if (!runnablePathInfo.exists()) {
        qInfo() << "WatchBPManager::isInstalled - runnablePath does not exist";
        return false;
    }

    if (!runnableNameInfo.exists() || !runnableNameInfo.isExecutable()) {
        qInfo() << "WatchBPManager::isInstalled - runnableName is not executable";
        return false;
    }

    if (!backupDatabasePathInfo.exists() || !backupDatabasePathInfo.isReadable()) {
        qInfo() << "WatchBPManager::isInstalled - backupDatabasePath is not readable";
        return false;
    }

    return true;
}

bool WatchBPManager::start()
{
    qCritical() << "WatchBPManager::start";

    QFile::remove(m_databasePath);

    QFile backupDatabaseFile(m_backupDatabasePath);
    if (!backupDatabaseFile.copy(m_databasePath)) {
        qCritical() << "WatchBPManager::start - could not copy backup database to data folder";
        QMessageBox::critical(nullptr, "Error", "Could not start WatchBP Analyzer. please contact support");
        return false;
    }

    qDebug() << "WatchBPManager::start - copied backup";

    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName(m_databasePath);

    if (!m_database.open()) {
        qCritical() << "WatchBPManager::start: " << m_database.lastError();
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO Patient "
                  "(Name, ID, Gender, DOB, Physician) values "
                  "(:name, :id, :sex, :dob, :physician)");

    query.bindValue(":name",   "Participant");
    query.bindValue(":id",     m_session->getBarcode());
    query.bindValue(":sex",    m_session->getInputData().value("sex").toString() == "F" ? 1 : 0);
    query.bindValue(":dob",    m_session->getInputData().value("dob").toString());
    query.bindValue(":physician", "CLSA");

    if (!query.exec()) {
        qCritical() << "Can't insert patient" << query.lastError();
        return false;
    }

    qDebug() << "WatchBPManager::start - update patient";

    QString command = m_runnableName;
    QStringList arguments;

    m_process.setProgram(command);
    m_process.setArguments(arguments);
    m_process.setWorkingDirectory(m_runnablePath);
    m_process.setProcessChannelMode(QProcess::ForwardedChannels);

    connect(&m_process, &QProcess::started, this, [this]() {
        qDebug() << "Process started: " << m_process.arguments().join(" ");
    });

    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &WatchBPManager::readOutput);

    connect(&m_process, &QProcess::errorOccurred, this,
            [](QProcess::ProcessError error) {
                QStringList s = QVariant::fromValue(error).toString().split(QRegExp("(?=[A-Z])"), Qt::SkipEmptyParts);
                qDebug() << "ERROR: process error occured: " << s.join(" ").toLower();
            });

    connect(&m_process, &QProcess::stateChanged, this,
            [](QProcess::ProcessState state) {
                QStringList s = QVariant::fromValue(state).toString().split(QRegExp("(?=[A-Z])"), Qt::SkipEmptyParts);
                qDebug() << "process state: " << s.join(" ").toLower();
            });

    m_process.start();

    return true;
}

void WatchBPManager::measure()
{
    qInfo() << "WatchBPManager::measure";
}

void WatchBPManager::finish()
{
    qInfo() << "WatchBPManager::finish";
}

void WatchBPManager::readOutput()
{
    qInfo() << "WatchBPManager::readOutput";

    QSqlQuery dataQuery;
    dataQuery.prepare("SELECT * FROM Data");
    if (!dataQuery.exec()) {
        qCritical() << "Query failed";
        return;
    }

    QJsonObject output;

    QJsonArray measurements;

    while (dataQuery.next()) {
        QJsonObject measurement {
            { "ID", 			dataQuery.value(0).toInt()       },
            { "Patient", 		dataQuery.value(1).toInt()       },
            { "SYS", 			dataQuery.value(2).toInt()       },
            { "DIA", 			dataQuery.value(3).toInt()       },
            { "MAP", 			dataQuery.value(4).toInt()       },
            { "PP", 			dataQuery.value(5).toInt()       },
            { "cSYS", 			dataQuery.value(7).toInt() 	     },
            { "cDIA", 			dataQuery.value(8).toInt()       },
            { "cPP", 			dataQuery.value(9).toInt()       },
            { "HR", 			dataQuery.value(12).toInt()      },
            { "AFIB", 			dataQuery.value(13).toInt()   	 },
            { "Spare1", 		dataQuery.value(14).toInt()   	 },
            { "CODE", 			dataQuery.value(15).toString()   },
            { "NOTE", 			dataQuery.value(16).toString()   },
            { "Condition", 		dataQuery.value(17).toInt()   	 },
            { "UpdateTime", 	dataQuery.value(12).toInt()   	 },
            { "Device", 		dataQuery.value(19).toInt()   	 },
            { "Date", 			dataQuery.value(20).toInt()   	 },
            { "BPM", 			dataQuery.value(21).toString()   },
            { "AwakeTime", 		dataQuery.value(22).toInt()   	 },
            { "AsleepTime", 	dataQuery.value(23).toInt()      },
            { "Interval1", 		dataQuery.value(24).toInt()      },
            { "Interval2", 		dataQuery.value(25).toInt()      },
            { "Interval1C", 	dataQuery.value(26).toInt()      },
            { "Interval2C", 	dataQuery.value(27).toInt()      },
            { "Option", 		dataQuery.value(28).toInt()      },
        };

        const int dataId = measurement.value("ID").toInt();

        QSqlQuery dataPVP {};

        dataPVP.prepare(
            "SELECT "
                "DataPVPWave.DataId, "
                "DataPVPWave.PVPWaveId, "
                "PVPWave.Index, "
                "PVPWave.Part, "
                "PVPWave.Type, "
                "PVPWave.Resolution, "
                "PVPWave.PointsBlobbed "
            "FROM DataPVPWave INNER JOIN PVPWave ON DataPVPWave.PVPWaveId = PVPWave.Index "
            "WHERE DataPVPWave.DataId = :dataId");

        dataPVP.bindValue(":dataId", dataId);
        if (!dataQuery.exec()) {
            qCritical() << "Query 2 failed";
            return;
        }

        QJsonArray pvp {};

        while (dataPVP.next()) {
            QJsonObject data = {
                { "DataId",        dataPVP.value(0).toInt()    },
                { "PVPWaveId",     dataPVP.value(1).toInt()    },
                { "Index",         dataPVP.value(2).toInt()    },
                { "Part",          dataPVP.value(3).toInt()    },
                { "Type",          dataPVP.value(4).toInt()    },
                { "Resolution",    dataPVP.value(5).toInt()    },
                { "PointsBlobbed", dataPVP.value(6).toString() },
            };

            pvp.append(data);
        }

        measurement.insert("pvp", pvp);

        measurements.append(measurement);
    }

    output["measurements"] = measurements;

    QString data = JsonSettings::serializeJson(output, true);

    QFile::remove("C:/Users/hoarea/cypress-builds/Cypress/watch_bp/output.txt");

    qDebug() << "write" << data;
    QFile file("C:/Users/hoarea/cypress-builds/Cypress/watch_bp/output.txt");
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream stream(&file);
        stream << data;
        file.close();
    }
}

void WatchBPManager::addManualMeasurement()
{
    qInfo() << "WatchBPManager::addManualMeasurement";
}

bool WatchBPManager::clearData()
{
    qInfo() << "WatchBPManager::clearData";
    return true;
}

bool WatchBPManager::cleanUp()
{
    qInfo() << "WatchBPManager::cleanUp";

    return true;
}

bool WatchBPManager::setUp()
{
    return false;
}
