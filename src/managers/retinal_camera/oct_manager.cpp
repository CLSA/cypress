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
    { "dataPath",        { "oct/dataPath",        Dir }},
    { "databaseName",    { "oct/database/name",   NonEmptyString }},
    { "databaseBackup",  { "oct/database/backup", File }},
}};


OCTManager::OCTManager(QSharedPointer<OCTSession> session): ManagerBase { session }
{
    m_runnableName   = config.getSetting("runnableName");
    m_runnablePath   = config.getSetting("runnablePath");
    m_exportPath     = config.getSetting("exportPath");
    m_dataPath       = config.getSetting("dataPath");
    m_databaseName   = config.getSetting("databaseName");
    m_databaseBackup = config.getSetting("databaseBackup");

    m_test.reset(new OCTTest);
}

OCTManager::~OCTManager()
{

}

bool OCTManager::start()
{
    // Check if chrome is already running

    qDebug() << "OCTManager::start - connect to database";


    m_db = QSqlDatabase::addDatabase("QODBC");
    m_db.setDatabaseName(m_databaseName);

    if (!m_db.open()) {
        qCritical() << "OCTManager::start - could not connect to database";
        return false;
    }

    if (!m_db.transaction()) {
        qCritical() << "Couldn't start transaction" << m_db.lastError();
        return false;
    }
    QSqlQuery query;
    query.prepare("ALTER DATABASE [IMAGEnet] SET single_user with rollback immediate");
    if (!query.exec()) {
        qCritical() << "Couldn't set database to single_user";
        return false;
    }
    query.prepare("RESTORE DATABASE [IMAGEnet] FROM DISK = N'C:\\Users\\Public\\Documents\\oct.bak' WITH FILE = 1, NOUNLOAD, STATS = 5");
    if (!query.exec()) {
        qCritical() << "Couldn't restore database";
        return false;
    }

    query.prepare("ALTER DATABASE [IMAGEnet] SET multi_user");
    if (!query.exec()) {
        qCritical() << "Couldn't set database to multi_user";
        return false;
    }

    if (!m_db.commit()) {
        qCritical() << "Couldn't commit " << query.lastError();
        return false;
    }

    // Remove old files
    qDebug() << "OCTManager::start - remove old files";
    if (!FileUtils::clearDirectory(m_exportPath))
    {
        qCritical() << "OCTManager::start - could not clear export directory";
        return false;
    }

    // Remove FDA files from Data
    if (!FileUtils::clearDirectory(m_dataPath))
    {
        qCritical() << "OCTManager::start - could not clear data directory";
    }

    // Prepare database
    m_db.transaction();
    qDebug() << "OCTManager::start - inserting person";
    query.prepare("INSERT INTO IMAGEnet.dbo.Persons (PersonUid, SurName, ForeName) VALUES (:personUid, :firstName, :lastName)");
    query.bindValue(":personUid", defaultPersonUUID);
    query.bindValue(":firstName", "CLSA");
    query.bindValue(":lastName", "Participant");
    if (!query.exec()) {
        qCritical() << query.lastError().text();
        return false;
    }
    qDebug() << "OCTManager::start - inserting patient";
    query.prepare("INSERT INTO IMAGEnet.dbo.Patients (PatientUid, PatientIdentifier, PersonUid) VALUES (:patientUid, :participantId, :personUUID)");
    query.bindValue(":patientUid", defaultPatientUUID);
    query.bindValue(":participantId", m_session->getBarcode());
    query.bindValue(":personUUID", defaultPatientUUID);
    if (!query.exec()) {
        qCritical() << "OCTManager::start - " << query.lastError().text();
        return false;
    }
    m_db.commit();

    // Setup process
    configureProcess();
    m_process.start();

    if (!m_process.waitForStarted())
        return false;

    emit started(m_test);
    emit dataChanged(m_test);
    emit canMeasure();

    // Listen to directory changes
    m_directoryWatcher.reset(new DicomDirectoryWatcher(m_exportPath));

    // Whenever it changes, get the files
    connect(m_directoryWatcher.get(),
            &DicomDirectoryWatcher::dicomDirectoryChanged,
            this,
            &OCTManager::readOutput);

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
}

void OCTManager::readOutput()
{
    QDir exportDir(m_exportPath);

    QFileInfoList entries = exportDir.entryInfoList();

    QList<QJsonObject> filePaths;

    foreach (auto entry, entries)
    {
        if (entry.suffix() == "dcm")
        {
            qDebug() << entry.absoluteFilePath();
            filePaths.append({ { "path", entry.absoluteFilePath() }, { "name", entry.fileName() } });
        }
    }

    m_test->setFiles(filePaths);

    if (m_test->getFiles().keys().length() >= 2) {
        finish();
    }
}

void OCTManager::finish()
{
    qDebug() << "OCTManager::finish";
    ManagerBase::finish();
}
