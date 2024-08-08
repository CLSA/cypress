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
    { "runnableName",        { "watch_bp/runnableName",       Exe            }},
    { "runnablePath",        { "watch_bp/runnablePath",       Dir            }},
    { "databasePath",        { "watch_bp/databasePath",       File           }},
    { "backupDatabasePath",  { "watch_bp/backupDatabasePath", File           }},
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
    qDebug() << "WatchBPManager::start";
    emit started(m_test);

    if (!restoreBackup())
        return false;

    if (!configureDatabase())
        return false;

    if (!addPatient())
        return false;

    if (!configureProcess())
        return false;

    m_process.start();

    return true;
}

bool WatchBPManager::restoreBackup()
{
    qDebug() << "WatchBPManager::restoreBackup";
    QFile::remove(m_databasePath);
    QFile backupDatabaseFile(m_backupDatabasePath);
    if (!backupDatabaseFile.copy(m_databasePath)) {
        qCritical() << "WatchBPManager::restoreBackup - could not copy backup database to data folder";
        QMessageBox::critical(nullptr, "Error", "Could not start WatchBP Analyzer. please contact support");
        return false;
    }

    return true;
}

bool WatchBPManager::configureDatabase()
{
    qDebug() << "WatchBPManager::configureDatabase";
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName(m_databasePath);

    if (!m_database.open()) {
        qCritical() << "WatchBPManager::configureDatabase: " << m_database.lastError();
        return false;
    }

    return true;
}

bool WatchBPManager::addPatient()
{
    qDebug() << "WatchBPManager::addPatient";
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

    return true;
}

bool WatchBPManager::configureProcess()
{
    qDebug() << "WatchBPManager::configureProcess";

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

    return true;
}

void WatchBPManager::measure()
{
    qDebug() << "WatchBPManager::measure";
    m_test->reset();

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
            { "ID", 			dataQuery.value(0).toJsonValue()  },
            { "Patient", 		dataQuery.value(1).toJsonValue()  },
            { "SYS", 			dataQuery.value(2).toJsonValue()  },
            { "DIA", 			dataQuery.value(3).toJsonValue()  },
            { "MAP", 			dataQuery.value(4).toJsonValue()  },
            { "PP", 			dataQuery.value(5).toJsonValue()  },
            { "Spare5", 		dataQuery.value(6).toJsonValue()  },
            { "cSYS", 			dataQuery.value(7).toJsonValue()  },
            { "cDIA", 			dataQuery.value(8).toJsonValue()  },
            { "cPP", 			dataQuery.value(9).toJsonValue()  },
            { "Spare3", 		dataQuery.value(10).toJsonValue() },
            { "Spare4", 		dataQuery.value(11).toJsonValue() },
            { "HR", 			dataQuery.value(12).toJsonValue() },
            { "AFIB", 			dataQuery.value(13).toJsonValue() },
            { "Spare1", 		dataQuery.value(14).toJsonValue() },
            { "CODE", 			dataQuery.value(15).toJsonValue() },
            { "NOTE", 			dataQuery.value(16).toJsonValue() },
            { "Condition", 		dataQuery.value(17).toJsonValue() },
            { "UpdateTime", 	dataQuery.value(18).toJsonValue() },
            { "Device", 		dataQuery.value(19).toJsonValue() },
            { "Date", 			dataQuery.value(20).toJsonValue() },
            { "BPM", 			dataQuery.value(21).toJsonValue() },
            { "AwakeTime", 		dataQuery.value(22).toJsonValue() },
            { "AsleepTime", 	dataQuery.value(23).toJsonValue() },
            { "Interval1", 		dataQuery.value(24).toJsonValue() },
            { "Interval2", 		dataQuery.value(25).toJsonValue() },
            { "Interval1C", 	dataQuery.value(26).toJsonValue() },
            { "Interval2C", 	dataQuery.value(27).toJsonValue() },
            { "Option", 		dataQuery.value(28).toJsonValue() },
            { "Spare2", 		dataQuery.value(29).toJsonValue() },
            { "Spare6", 		dataQuery.value(30).toJsonValue() },
            { "Spare7", 		dataQuery.value(31).toJsonValue() },
            { "Spare8", 		dataQuery.value(32).toJsonValue() },
            { "Spare9", 		dataQuery.value(33).toJsonValue() },
            { "Spare10", 		dataQuery.value(34).toJsonValue() },
            { "Spare11", 		dataQuery.value(35).toJsonValue() },
            { "Spare12", 		dataQuery.value(36).toJsonValue() },
            { "Spare13", 		dataQuery.value(37).toJsonValue() },
            { "Spare14", 		dataQuery.value(38).toJsonValue() },
            { "Spare15", 		dataQuery.value(39).toJsonValue() },
            { "Spare16", 		dataQuery.value(40).toJsonValue() },
            { "Spare17", 		dataQuery.value(41).toJsonValue() },
            { "Spare18", 		dataQuery.value(42).toJsonValue() },
            { "Spare19", 		dataQuery.value(43).toJsonValue() },
            { "Spare20", 		dataQuery.value(44).toJsonValue() },
        };

        //const int dataId = measurement.value("ID").toInt();

        //QSqlQuery dataPVP {};

        //dataPVP.prepare(
        //    "SELECT "
        //        "DataPVPWave.DataId, "
        //        "DataPVPWave.PVPWaveId, "
        //        "PVPWave.Index, "
        //        "PVPWave.Part, "
        //        "PVPWave.Type, "
        //        "PVPWave.Resolution, "
        //        "PVPWave.PointsBlobbed "
        //    "FROM DataPVPWave INNER JOIN PVPWave ON DataPVPWave.PVPWaveId = PVPWave.Index "
        //    "WHERE DataPVPWave.DataId = :dataId");

        //dataPVP.bindValue(":dataId", dataId);
        //if (!dataPVP.exec()) {
        //    qCritical() << "Query 2 failed";
        //    return;
        //}

        //QJsonArray pvp {};

        //while (dataPVP.next()) {
        //    QJsonObject data = {
        //        { "DataId",        dataPVP.value(0).toJsonValue() },
        //        { "PVPWaveId",     dataPVP.value(1).toJsonValue() },
        //        { "Index",         dataPVP.value(2).toJsonValue() },
        //        { "Part",          dataPVP.value(3).toJsonValue() },
        //        { "Type",          dataPVP.value(4).toJsonValue() },
        //        { "Resolution",    dataPVP.value(5).toJsonValue() },
        //        { "PointsBlobbed", dataPVP.value(6).toJsonValue() },
        //    };

        //    pvp.append(data);
        //}

        //measurement.insert("pvp", pvp);
        measurements.append(measurement);
    }

    output["measurements"] = measurements;

    auto test = qSharedPointerCast<WatchBPTest>(m_test);

    qDebug().noquote() << output;

    test->fromJson(output);

    emit dataChanged(test);
    checkIfFinished();

    qDebug().noquote() << JsonSettings::prettyPrintJson(test->toJsonObject());
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

    ManagerBase::finish();
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
    bpm->setAttribute("Date", QDateTime::currentDateTimeUtc());

    test->addMeasurement(bpm);
    test->updateAverage();

    qDebug() << test->toJsonObject();

    emit dataChanged(m_test);
    checkIfFinished();
}

void WatchBPManager::removeMeasurement(const int index)
{
    qInfo() << "WatchBPManager::removeMeasurement: " << index;
    auto test = qSharedPointerCast<WatchBPTest>(m_test);

    test->removeMeasurement(index);
    test->updateAverage();

    emit dataChanged(m_test);
    checkIfFinished();
}
