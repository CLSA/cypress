#include "watch_bp_manager.h"

#include <QJsonObject>
#include <QFile>
#include <QDir>

WatchBPManager::WatchBPManager(QSharedPointer<CypressSession> session): ManagerBase(session)
{
    m_runnableName = CypressSettings::readSetting("watch_bp/runnableName").toString();
    m_runnablePath = CypressSettings::readSetting("watch_bp/runnablePath").toString();
    m_databasePath = CypressSettings::readSetting("watch_bp/databasePath").toString();
}

bool WatchBPManager::isInstalled()
{
    const QString runnableName = CypressSettings::readSetting("watch_bp/runnableName").toString();
    const QString runnablePath = CypressSettings::readSetting("watch_bp/runnablePath").toString();
    const QString databasePath = CypressSettings::readSetting("watch_bp/databasePath").toString();

    if (runnableName.isEmpty() || runnableName.isNull()) {
        qInfo() << "RetinalCamera: runnableName is not defined";
        return false;
    }

    if (runnablePath.isEmpty() || runnablePath.isNull()) {
        qInfo() << "RetinalCamera: runnablePath is not defined";
        return false;
    }

    if (databasePath.isEmpty() || databasePath.isNull()) {
        qInfo() << "RetinalCamera: databasePath is not defined";
        return false;
    }

    const QDir runnablePathInfo(runnableName);
    const QFileInfo runnableNameInfo(runnableName);
    const QFileInfo databasePathInfo(databasePath);

    if (!runnablePathInfo.exists()) {
        qInfo() << "RetinalCamera: runnablePath does not exist";
        return false;
    }

    if (!runnableNameInfo.exists() || !runnableNameInfo.isExecutable()) {
        qInfo() << "RetinalCamera: runnableName is not executable";
        return false;
    }

    if (!databasePathInfo.exists() || !databasePathInfo.isReadable()) {
        qInfo() << "RetinalCamera: databasePath is not readable";
        return false;
    }

    return true;
}

bool WatchBPManager::start()
{
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName(m_databasePath);

    if (!m_database.open()) {
        qCritical() << "WatchBPManager::start: " << m_database.lastError();
        return false;
    }

    QSqlQuery query;

    query.prepare("DELETE FROM Data");
    if (!query.exec()) {
        qCritical() << "WatchBPManager::start: " << query.lastError();
        return false;
    }

    query.prepare("DELETE FROM DataPVPWave");
    if (!query.exec()) {
        qCritical() << "WatchBPManager::start: " << query.lastError();
        return false;
    }

    query.prepare("DELETE FROM Info");
    if (!query.exec()) {
        qCritical() << "WatchBPManager::start: " << query.lastError();
        return false;
    }

    query.prepare("DELETE FROM PVPWave");
    if (!query.exec()) {
        qCritical() << "WatchBPManager::start: " << query.lastError();
        return false;
    }

    query.prepare("DELETE FROM Patient");
    if (!query.exec()) {
        qCritical() << "WatchBPManager::start: " << query.lastError();
        return false;
    }

    //query.prepare("DELETE FROM Physician");
    //if (!query.exec()) {
    //    qCritical() << "WatchBPManager::start: " << query.lastError();
    //    return false;
    //}

    query.prepare("INSERT INTO Patient "
                  "(Name, ID, Gender, Age, DOB, Physician) values "
                  "(:name, :id, :gender, :dob, :physician)");

    query.bindValue(":name", "CLSA Participant");
    query.bindValue(":id", "12345678");
    query.bindValue(":gender", "M");
    query.bindValue(":dob", "1995-12-06");
    query.bindValue(":physician", "None");

    qInfo() << "WatchBPManager::start";
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
        return;
    }

    while (dataQuery.next()) {
        QJsonObject measurement {
            { "ID", 			dataQuery.value(0).toInt()       },
            { "Patient", 		dataQuery.value(1).toInt()       },
            { "SYS", 			dataQuery.value(2).toInt()       },
            { "DIA", 			dataQuery.value(3).toInt()       },
            { "MAP", 			dataQuery.value(4).toInt()       },
            { "PP", 			dataQuery.value(5).toInt()       },
            { "Spare5", 		dataQuery.value(6).toFloat()     },
            { "cSYS", 			dataQuery.value(7).toInt() 	     },
            { "cDIA", 			dataQuery.value(8).toInt()       },
            { "cPP", 			dataQuery.value(9).toInt()       },
            { "Spare3", 		dataQuery.value(10).toInt()      },
            { "Spare4", 		dataQuery.value(11).toFloat()    },
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
            { "Spare2", 		dataQuery.value(29).toString()   },
            { "Spare6", 		dataQuery.value(30).toString()   },
            { "Spare7", 		dataQuery.value(31).toInt()      },
            { "Spare8", 		dataQuery.value(32).toInt()      },
            { "Spare9", 		dataQuery.value(33).toInt()      },
            { "Spare10", 		dataQuery.value(34).toInt()      },
            { "Spare11", 		dataQuery.value(35).toString()   },
            { "Spare12", 		dataQuery.value(36).toString()   },
            { "Spare13", 		dataQuery.value(37).toString()   },
            { "Spare14", 		dataQuery.value(38).toString()   },
            { "Spare15", 		dataQuery.value(39).toString()   },
            { "Spare16", 		dataQuery.value(40).toString()   },
            { "Spare17", 		dataQuery.value(41).toString()   },
            { "Spare18", 		dataQuery.value(42).toString()   },
            { "Spare19", 		dataQuery.value(43).toString()   },
            { "Spare20", 		dataQuery.value(44).toString()   },
        };

        QSqlQuery dataPVP;
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
