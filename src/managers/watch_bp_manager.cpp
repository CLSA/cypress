#include "watch_bp_manager.h"
#include "auxiliary/json_settings.h"

#include "data/blood_pressure/tests/watch_bp_test.h"
#include "data/blood_pressure/measurements/watch_bp_measurement.h"

#include <QMessageBox>
#include <QJsonObject>
#include <QJsonArray>
#include <QSqlError>

#include <QFile>
#include <QDir>

#include <windows.h>
#include <TlHelp32.h>

DeviceConfig WatchBPManager::config {{
    { "processName",         { "watch_bp/processName",        NonEmptyString }},
    { "runnableName",        { "watch_bp/runnableName",       Exe }},
    { "runnablePath",        { "watch_bp/runnablePath",       Dir }},
    { "databasePath",        { "watch_bp/databasePath",       File }},
    { "backupDatabasePath",  { "watch_bp/backupDatabasePath", File }},
}};

WatchBPManager::WatchBPManager(QSharedPointer<WatchBPSession> session): ManagerBase(session)
{
    m_processName = config.getSetting("processName");
    m_runnableName = config.getSetting("runnableName");
    m_runnablePath = config.getSetting("runnablePath");
    m_databasePath = config.getSetting("databasePath");
    m_backupDatabasePath = config.getSetting("backupDatabasePath");

    m_test.reset(new WatchBPTest);
}

bool WatchBPManager::start()
{
    emit started(m_test);

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

    connect(
        &m_process,
        &QProcess::started,
        this,
        [this]() {
            qDebug() << "Process started: " << m_process.arguments().join(" ");
    });

    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [this]() {
        emit canMeasure();
    });

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

    QFile::remove("C:/Users/hoarea/cypress-builds/Cypress/watch_bp/output.json");

    qDebug() << "write" << data;
    QFile file("C:/Users/hoarea/cypress-builds/Cypress/watch_bp/output.json");
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream stream(&file);
        stream << data;
        file.close();
    }
}

void WatchBPManager::finish()
{
    qInfo() << "WatchBPManager::finish";

    auto test = qSharedPointerCast<WatchBPTest>(m_test);
    test->updateAverage();

    if (test->getMeasurementCount() < 2) {
        qWarning() << "WatchBPManager::finish - invalid test, this should not occur";
        m_test->reset();
        QMessageBox::critical(nullptr, "Unexpected error", "An unexpected error occurred, please use manual entry or try again");
        return;
    }
}

void WatchBPManager::addManualEntry(const int systolic, const int diastolic, const int pulse)
{
    qInfo() << "WatchBPManager::addManualMeasurement";
    auto test = qSharedPointerCast<WatchBPTest>(m_test);

    QSharedPointer<WatchBPMeasurement> bpm(new WatchBPMeasurement());
    bpm->setAttribute("ID", m_test->getMeasurementCount() + 1);
    bpm->setAttribute("SYS", systolic);
    bpm->setAttribute("DIA", diastolic);
    bpm->setAttribute("PP", pulse);
    bpm->setAttribute("StartTime", QDateTime::currentDateTimeUtc());
    bpm->setAttribute("EndTime", QDateTime::currentDateTimeUtc());

    test->addMeasurement(bpm);
    test->updateAverage();

    qDebug() << test->toJsonObject();

    emit dataChanged(m_test);

    if (m_test->isValid()) {
        emit canFinish();
    }
    else {
        emit cannotFinish();
    }
}

void WatchBPManager::removeMeasurement(const int index)
{
    qInfo() << "WatchBPManager::removeMeasurement: " << index;
    auto test = qSharedPointerCast<WatchBPTest>(m_test);

    test->removeMeasurement(index);
    test->updateAverage();

    emit dataChanged(m_test);
    if (m_test->isValid()) {
        emit canFinish();
    }
    else {
        emit cannotFinish();
    }
}
