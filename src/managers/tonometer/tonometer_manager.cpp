#include "cypress_settings.h"

#include "tonometer_manager.h"
#include "server/sessions/tonometer_session.h"

#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardItemModel>

TonometerManager::TonometerManager(QSharedPointer<TonometerSession> session)
    : ManagerBase(session)
{
    m_test.reset(new TonometerTest);
    m_test->setExpectedMeasurementCount(2);


    m_runnableName = CypressSettings::readSetting("tonometer/runnableName").toString();
    m_runnablePath = CypressSettings::readSetting("tonometer/runnablePath").toString();
    m_databasePath = CypressSettings::readSetting("tonometer/databasePath").toString();
    m_temporaryPath = CypressSettings::readSetting("tonometer/temporaryPath").toString();

    m_db = QSqlDatabase::addDatabase("QODBC");
    m_db.setDatabaseName("Driver={Microsoft Access Driver (*.mdb)};DBQ=" + QDir::toNativeSeparators(m_databasePath));
}

TonometerManager::~TonometerManager()
{
    m_db.close();
}

bool TonometerManager::start()
{
    qInfo() << "TonometerManager::start";

    if (!backupData()) {
        qCritical() << "TonometerManager::start: could not backup data";

        return false;
    }

    if (!clearData()) {
        qCritical() << "TonometerManager::start: could not clear data";

        emit error("Something went wrong");
        return false;
    }

    if (!setUp()) {
        qCritical() << "TonometerManager::start: could not backup data";

        emit error("Something went wrong");
        return false;
    }

    measure();

    return true;
}


bool TonometerManager::isInstalled()
{
    qInfo() << "TonometerManager::isInstalled";

    const QString runnableName = CypressSettings::readSetting("tonometer/runnableName").toString();
    const QString runnablePath = CypressSettings::readSetting("tonometer/runnablePath").toString();
    const QString databasePath = CypressSettings::readSetting("tonometer/databasePath").toString();
    const QString temporaryPath = CypressSettings::readSetting("tonometer/temporaryPath").toString();

    if (runnablePath.isNull() || runnablePath.isEmpty()) {
        qInfo() << "TonometerManager::isInstalled: runnablePath is not defined";
        return false;
    }

    if (runnableName.isNull() || runnableName.isEmpty()) {
        qInfo() << "TonometerManager::isInstalled: runnableName is not defined";
        return false;
    }

    if (databasePath.isNull() || databasePath.isEmpty()) {
        qInfo() << "TonometerManager::isInstalled: databasePath is not defined";
        return false;
    }

    if (temporaryPath.isNull() || temporaryPath.isEmpty()) {
        qInfo() << "TonometerManager::isInstalled: temporaryPath is not defined";
        return false;
    }

    const QDir runnablePathInfo(runnablePath);
    if (!runnablePathInfo.exists()) {
        qInfo() << "TonometerManager::isInstalled: runnable does not exist";
        return false;
    }

    const QFileInfo runnableNameInfo(runnableName);
    if (!runnableNameInfo.exists()) {
        qInfo() << "TonometerManager::isInstalled: runnable does not exist";
        return false;
    }
    if (!runnableNameInfo.isExecutable()) {
        qInfo() << "TonometerManager::isInstalled: runnable is not executable";
        return false;
    }

    const QFileInfo databaseFileInfo(databasePath);
    if (!databaseFileInfo.isFile()) {
        qInfo() << "TonometerManager::isInstalled: database is not a file";
        return false;
    }

    if (!databaseFileInfo.isReadable()) {
        qInfo() << "TonometerManager::isInstalled: database is not readable";
        return false;
    }

    const QDir backupDir(temporaryPath);
    if (!backupDir.exists()) {
        qInfo() << "TonometerManager::isInstalled: backup dir does not exist";
        return false;
    }

    return true;
}

void TonometerManager::measure()
{
    qInfo() << "TonometerManager::measure";

    if (m_process.state() != QProcess::NotRunning) {
        emit error("ORA is already running");
        return;
    }

    m_process.start();

    if (!m_process.waitForStarted()) {
        emit error("Could not start the ORA application");
        return;
    }
}

void TonometerManager::readOutput()
{
    qInfo() << "TonometerManager::readOutput";

    const QVariantMap leftResults = extractMeasures("L");
    const QVariantMap rightResults = extractMeasures("R");

    const QList<QVariantMap> results { leftResults, rightResults };

    auto test = qSharedPointerCast<TonometerTest>(m_test);
    test->fromVariantMapList(results);

    finish();
}

void TonometerManager::configureProcess()
{
    qInfo() << "TonometerManager::configureProcess";

    m_process.setProgram(m_runnableName);
    m_process.setWorkingDirectory(m_runnablePath);
    m_process.setProcessChannelMode(QProcess::ForwardedChannels);

    // connect signals and slots to QProcess one time only
    //
    connect(&m_process, &QProcess::started,
        this, [this]() {
            qDebug() << "process started: " << m_process.arguments().join(" ");
        });

    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        this, &TonometerManager::readOutput);

    connect(&m_process, &QProcess::errorOccurred,
        this, [](QProcess::ProcessError error)
        {
            QStringList s = QVariant::fromValue(error).toString().split(QRegExp("(?=[A-Z])"), Qt::SkipEmptyParts);
            qDebug() << "ERROR: process error occured: " << s.join(" ").toLower();
        });

    connect(&m_process, &QProcess::stateChanged,
        this, [](QProcess::ProcessState state) {
            QStringList s = QVariant::fromValue(state).toString().split(QRegExp("(?=[A-Z])"), Qt::SkipEmptyParts);
            qDebug() << "process state: " << s.join(" ").toLower();
        });
}


bool TonometerManager::clearData()
{
    qInfo() << "TonometerManager::clearData";

    m_test->reset();
    restoreDatabase();

    return true;
}

bool TonometerManager::backupData() {
    const QFileInfo databasePath(m_databasePath);
    const QDir backupPath(m_temporaryPath);

    if (!QFile::exists(databasePath.absoluteFilePath()) ) {
        qCritical() << "tonometer database file does not exist at path" << databasePath.absoluteFilePath();
        return false;
    }

    const QString backupDatabasePath = backupPath.absoluteFilePath(databasePath.fileName());
    if (QFileInfo::exists(backupDatabasePath)) {
        if (!QFile::remove(backupDatabasePath)) {
            qCritical() << "tonometer backup exists but cannot be removed..";
            return false;
        }
    }

    if (!QFile::copy(databasePath.absoluteFilePath(), backupDatabasePath)) {
        qCritical() << "could not copy database file at " << databasePath.absoluteFilePath() << "to" << backupDatabasePath;
        return false;
    }

    return true;
}

bool TonometerManager::restoreData() {
    //QDir backupPath(m_temporaryPath);
    //QFileInfo databasePath(m_databasePath);

    //QString backupDatabasePath = backupPath.absoluteFilePath(databasePath.fileName());
    //if (!backupPath.exists() ) {
    //    return false;
    //}

    //if (!QFile::remove(m_databasePath)) {
    //    return false;
    //}

    //if(!QFile::copy(backupDatabasePath, databasePath.absoluteFilePath())) {
    //    return false;
    //}

    return true;
}

bool TonometerManager::setUp()
{
    qInfo() << "TonometerManager::setUp";

    configureProcess();

    if (!m_db.open())
        return false;

    bool ok = insertPatient(
        m_session->getBarcode() + "," + "CLSA",
        m_session->getInputData()["date_of_birth"].toString(),
        m_session->getInputData()["sex"].toString(),
        m_session->getBarcode().toInt()
    );

    m_db.close();

    if (!ok) {
        qDebug() << "could not insert patient";
        return false;
    }

    return true;
}

bool TonometerManager::cleanUp()
{
    qInfo() << "TonometerManager::cleanUp";

    if (QProcess::NotRunning != m_process.state())
        m_process.close();

    if (!restoreDatabase())
        return false;

    if (!restoreData()) {
        qCritical() << "TonometerManager::cleanUp: could not restore database";
        return false;
    }

    return true;
}

bool TonometerManager::insertPatient(
                                     const QString &name,
                                     const QString &birthDate,
                                     const QString &sex,
                                     const int id)
{
    Q_UNUSED(id);
    Q_UNUSED(birthDate);

    qInfo() << "TonometerManager::insertPatient";

    QSqlQuery query(m_db);

    query.prepare("INSERT INTO Patients ( Name, BirthDate, Sex, GroupID, ID, RaceID ) VALUES ( :name, :birthDate, :sex, :groupId, :id, :raceId )");
    query.bindValue(":name", name);
    query.bindValue(":birthDate", "12/06/2000");
    query.bindValue(":sex", sex.startsWith("m", Qt::CaseSensitivity::CaseInsensitive));
    query.bindValue(":groupId", 2);
    query.bindValue(":id", 1234);
    query.bindValue(":raceId", 1);

    if (!query.exec()) {
        qCritical() << "Database error:" << m_db.lastError().text();
        return false;
    }

    return true;
}

QVariantMap TonometerManager::extractMeasures(const QString &eye)
{
    qInfo() << "TonometerManager::extractMeasures";

    if (!m_db.isOpen()) {
        if (!m_db.open()) {
            throw QException();
        }
    }

    QSqlQuery query(m_db);

    query.prepare("SELECT PatientID from Patients where ID = :id");
    query.bindValue(":id", 1234);

    if (!query.exec()) {
        qWarning() << "Database error:" << m_db.lastError().text();
    }

    query.first();

    const int patientId = query.value("PatientID").toInt();

    query.prepare("SELECT * from Measures where PatientID = :patientId and Eye = :eye ORDER BY MeasureDate desc");

    query.bindValue(":patientId", patientId);
    query.bindValue(":eye", QString(eye));

    QVariantMap resultMap;
    if (!query.exec()) {
        qWarning() << "Database error:" << m_db.lastError().text();
        return resultMap;
    }

    while (query.next()) {
        for (int i = 0; i < query.record().count(); ++i) {
            resultMap.insert(query.record().fieldName(i), query.value(i));
        }
    }

    return resultMap;
}

bool TonometerManager::restoreDatabase()
{
    qInfo() << "TonometerManager::restoreDatabase";

    if (!m_db.isOpen()) {
        if (!m_db.open()) {
            throw QException();
        }
    }

    QSqlQuery query(m_db);
    query.prepare("SELECT PatientID from Patients where ID = :id");
    query.bindValue(":id", 1234);

    if (!query.exec()) {
        qWarning() << "Database error:" << m_db.lastError().text();
    }

    query.first();

    const int patientId = query.value("PatientID").toInt();

    query.prepare("DELETE FROM Patients WHERE PatientID = :patientId");
    query.bindValue(":patientId", patientId);

    if (!query.exec()) {
        qWarning() << "Error deleting data from table:" << query.lastError().text();
        m_db.close();
        return false;
    }

    query.prepare("DELETE FROM Measures WHERE PatientID = :patientId");
    query.bindValue(":patientId", patientId);

    if (!query.exec()) {
        qWarning() << "Error deleting data from table:" << query.lastError().text();
        m_db.close();
        return false;
    }

    m_db.close();

    return true;
}
