#include "oct_manager.h"
#include "data/oct_test.h"
#include "server/sessions/oct_session.h"
#include "auxiliary/file_utils.h"

#include <QFileInfo>
#include <QMessageBox>

#include <QSqlQuery>
#include <QSqlError>


DeviceConfig OCTManager::config {{
    { "runnableName",    { "oct/runnableName",    Exe }},
    { "runnablePath",    { "oct/runnablePath",    Dir }},
    { "exportPath",      { "oct/exportPath",      Dir }},
    { "databaseName",    { "oct/database/name",   NonEmptyString }},
    { "databaseBackup",  { "oct/database/backup", File }},
}};


OCTManager::OCTManager(QSharedPointer<OCTSession> session): ManagerBase { session }
{
    m_runnableName   = config.getSetting("runnableName");
    m_runnablePath   = config.getSetting("runnablePath");
    m_exportPath     = config.getSetting("exportPath");
    m_databaseName   = config.getSetting("databaseName");
    m_databaseBackup = config.getSetting("databaseBackup");

    m_test.reset(new OCTTest);
}

OCTManager::~OCTManager() {}

bool OCTManager::start()
{
    // Check if chrome is already running

    // Connect to database
    qDebug() << "OCTManager::start - connect to database";
    m_db = QSqlDatabase::addDatabase("QODBC");
    m_db.setDatabaseName(m_databaseName);
    if (!m_db.open()) {
        qCritical() << "OCTManager::start - could not connect to database";
        return false;
    }

    // Restore database
    qDebug() << "OCTManager::start - restore database";
    QSqlQuery query(m_db);
    query.prepare("RESTORE DATABASE :databaseName FROM DISK = :databaseBackup WITH RECOVERY");
    query.bindValue(":databaseName", m_databaseName);
    query.bindValue(":databaseBackup", m_databaseBackup);

    if (!query.exec()) {
        qCritical() << "OCTManager::start - could not restore database";
        return false;
    }

    // Remove old files
    qDebug() << "OCTManager::start - remove old files";
    if (!FileUtils::clearDirectory(m_exportPath))
    {
        qCritical() << "OCTManager::start - could not clear export directory";
    }

    // Prepare database
    qDebug() << "OCTManager::start - inserting person";
    query.prepare("INSERT INTO Persons (PersonUid, SurName, ForeName) VALUES (:personUid, :firstName, :lastName");
    query.bindValue(":personUid", defaultPersonUUID);
    query.bindValue(":firstName", "CLSA");
    query.bindValue(":lastName", "Participant");
    if (!query.exec()) {
        qCritical() << query.lastError().text();
        return false;
    }

    qDebug() << "OCTManager::start - inserting patient";
    query.prepare("INSERT INTO Patients (PatientUid, PatientIdentifier, PersonUid) VALUES (:patientUid, :participantId, :personUUID");
    query.bindValue(":patientUUID", defaultPatientUUID);
    query.bindValue(":participantId", m_session->getBarcode());
    query.bindValue(":personUUID", defaultPatientUUID);
    if (!query.exec()) {
        qCritical() << "OCTManager::start - " << query.lastError().text();
        return false;
    }

    // Setup process
    qDebug() << "OCTManager::start - configure process";
    configureProcess();

    qDebug() << "OCTManager::start - start imagenet";
    m_process.start();
    if (!m_process.waitForStarted())
        return false;

    emit started(m_test);
    emit dataChanged(m_test);
    emit canMeasure();

    return true;
}

void OCTManager::configureProcess()
{
    qDebug() << "OCTManager::start - configureProcess";
    m_process.setProgram(m_runnableName);
    m_process.setWorkingDirectory(m_runnablePath);
    m_process.setProcessChannelMode(QProcess::ForwardedChannels);

    connect(&m_process, &QProcess::started, this, [=]() {
        qDebug() << "process started: " << m_process.arguments().join(" ");
    });

    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=]() {
        qDebug() << "process finished, reading output";
        readOutput();
    });
}

void OCTManager::readOutput()
{
    qDebug() << "OCTManager::measure";
    QSqlQuery query(m_db);

    query.prepare("SELECT * FROM OCTDSA WHERE PatientID = :patientId");
    if (!query.exec()) {
        qCritical() << "Select from OCTDSA failed";
        return;
    }

    if (!query.size()) {
        qWarning() << "Select returned 0 records";
    }

    query.prepare("SELECT * FROM Media WHERE PatientID = :patientId");
    if (!query.exec()) {
        qCritical() << "Select from media failed";
        return;
    }
    if (!query.size()) {
        qWarning() << "Select returned 0 records";
    }

    if (m_test->isValid()) {
        emit canFinish();
    }
}

void OCTManager::finish()
{
    qDebug() << "OCTManager::finish";
}
