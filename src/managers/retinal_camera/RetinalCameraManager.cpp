#include <QProcess>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QJsonDocument>
#include <QFileInfo>
#include <QSettings>
#include <QMessageBox>

#include "RetinalCameraManager.h"
#include "qsqlerror.h"


RetinalCameraManager::RetinalCameraManager(QWidget *parent)
    : ManagerBase{parent}
{
    QSettings qSettings;
}

// load and save device, paths and other constant settings to .ini
//
void RetinalCameraManager::loadSettings(const QSettings& settings)
{
    if (m_verbose)
        qDebug() << "RetinalCameraManager::loadSettings";

    m_runnableName      = settings.value("retinal_scan/exe").toString();
    m_defaultPatientUid = settings.value("instruments/retinal_scan/patient_id").toString();
    m_databaseDriver    = settings.value("instruments/retinal_scan/database/driver").toString();
    m_database		    = settings.value("instruments/retinal_scan/database/database").toString();
    m_databaseHost      = settings.value("instruments/retinal_scan/database/host").toString();
    m_databasePort      = settings.value("instruments/retinal_scan/database/port").toInt();
    m_databaseUser      = settings.value("instruments/retinal_scan/database/user").toString();
    m_databasePassword  = settings.value("instruments/retinal_scan/database/password").toString();

    if (m_verbose)
    {
        qInfo() << m_runnableName;
        qInfo() << m_defaultPatientUid;
        qInfo() << m_databaseVersion;
        qInfo() << m_databaseDriver;
        qInfo() << m_databaseHost;
        qInfo() << m_databasePort;
        qInfo() << m_databaseUser;
        qInfo() << m_databasePassword;
    }
}

void RetinalCameraManager::printDatabaseParams()
{
    qInfo() << m_runnableName;
    qInfo() << m_defaultPatientUid;
    qInfo() << m_databaseVersion;
    qInfo() << m_databaseDriver;
    qInfo() << m_databaseHost;
    qInfo() << m_databasePort;
    qInfo() << m_databaseUser;
    qInfo() << m_databasePassword;
}

void RetinalCameraManager::saveSettings(QSettings*) const
{
    if (m_verbose)
        qDebug() << "RetinalCameraManager::saveSettings";
}


void RetinalCameraManager::setInputData(const QVariantMap &data)
{
    if (m_verbose)
        qDebug() << "RetinalCameraManager::setInputData";

    m_inputData = data;
}

// collate test results and device and other meta data
// for the main application to write to .json
//
QJsonObject RetinalCameraManager::toJsonObject() const
{
    if (m_verbose)
        qDebug() << "RetinalCameraManager::toJsonObject";

    return QJsonDocument::fromVariant(m_inputData).object();
}

// build a model from test and measurement data for UI display
// of the results
//
void RetinalCameraManager::initializeModel()
{
    if (m_verbose)
        qDebug() << "RetinalCameraManager::initializeModel";
}

// update the model whenever the test data changes
//
void RetinalCameraManager::updateModel()
{
    if (m_verbose)
        qDebug() << "RetinalCameraManager::updateModel";
}

bool RetinalCameraManager::backupDatabase()
{
    return false;
}

bool RetinalCameraManager::openDatabase()
{
    // Open DB connection and clean up database
    QSqlDatabase db = QSqlDatabase::addDatabase(m_databaseDriver);
    db.setHostName(m_databaseHost);
    db.setPort(m_databasePort);
    db.setDatabaseName(m_database);
    db.setUserName(m_databaseUser);
    db.setPassword(m_databasePassword);

    bool ok = db.open();
    if (!ok)
    {
        qDebug() << "Could not open MS SQL database: " << db.lastError();
        printDatabaseParams();
    }

    return ok;
}

bool RetinalCameraManager::cleanupDatabase()
{
    // Remove data from relevant tables, and remove images from disk to prepare for a new test
    QSqlQuery query;
    QSqlQuery locationQuery;

    QString currentFileName;
    QString currentFileExtension;
    QString storagePathUid;

    bool ok = query.exec("SELECT FileName, FileExt, StoragePathUid FROM dbo.Media WHERE PatientUid = '" + m_defaultPatientUid + "'");
    if (!ok)
    {
        if (m_verbose)
        {
            qDebug() << query.lastQuery();
            qDebug() << query.lastError();
        }

        return false;
    }

    while (query.next())
    {
        currentFileName = query.value(0).toString();
        currentFileExtension = query.value(1).toString();
        storagePathUid = query.value(2).toString();

        ok = locationQuery.exec("SELECT Location FROM dbo.StoragePaths WHERE StoragePathUid = '" + storagePathUid + "'");
        if (!ok)
        {
            qDebug() << query.lastError();
            return false;
        }

        while (locationQuery.next())
        {
            QString location = locationQuery.value(0).toString();
            if (m_verbose)
            {
                qDebug() << "Deleting: " << location << "/" << currentFileName + currentFileExtension;
            }
        }

        if (m_verbose)
        {
            qDebug() << currentFileName << " " << currentFileExtension << " " << storagePathUid;
        }
    }

    ok = query.exec("DELETE FROM dbo.Exams WHERE PatientUid = '" + m_defaultPatientUid + "'");
    if (!ok)
    {
        qDebug() << query.lastError();
        return false;
    }

    ok = query.exec("DELETE FROM dbo.Media WHERE PatientUid = '" + m_defaultPatientUid + "'");
    if (!ok) {
        qDebug() << query.lastError();
        return false;
    }

    ok = query.exec("DELETE FROM dbo.Patients WHERE PatientUid = '" + m_defaultPatientUid + "'");
    if (!ok) {
        qDebug() << query.lastError();
        return false;
    }

    ok = query.exec("DELETE FROM dbo.Persons WHERE PersonUid = '" + m_defaultPatientUid + "'");
    if (!ok) {
        qDebug() << query.lastError();
        return false;
    }

    return true;
}

bool RetinalCameraManager::restoreDatabase()
{
    return false;
}

bool RetinalCameraManager::initializeDatabase()
{
    QSqlQuery query;
    bool ok = query.exec("INSERT INTO dbo.Persons (PersonUid, SurName, ForeName) values('11111111-2222-3333-4444-555555555555', 'Study', 'Participant')");
    if (!ok)
    {
        qDebug() << query.lastError();
        return false;
    }

    ok = query.exec("INSERT INTO dbo.Patients (PatientUid, PatientIdentifier, PersonUid) values('11111111-2222-3333-4444-555555555555', '11111111-2222-3333-4444-555555555555', '11111111-2222-3333-4444-555555555555')");
    if (!ok)
    {
        qDebug() << query.lastError();
        return false;
    }

    return true;
}

bool RetinalCameraManager::startRetinalCamera()
{
    m_process.setProgram(m_runnableName);
    m_process.start();

    bool started = m_process.waitForStarted();
    if (!started)
    {
        if (m_verbose)
            qDebug() << "RetinalCameraManager::startRetinalCamera: IMAGEnet_R4 did not start";
    }

    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [this]
    {
        finish();
    });

    return started;
}

void getStoragePathQueryString()
{

}

void RetinalCameraManager::start()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
    QFileInfo info(m_runnableName);
    QStringList arguments;

    bool ok = false;

    if (m_verbose)
    {
        qDebug() << "RetinalCameraManager::start";
    }

    loadSettings(settings);

    ok = openDatabase();
    if (!ok)
    {
        QMessageBox::critical(m_parent, tr("Cypress"),
            tr("Something went wrong, Cypress was unable to access the retinal camera")
        );

        return;
    }

    ok = cleanupDatabase();
    if (!ok)
    {
        QMessageBox::critical(m_parent, tr("Cypress"),
            tr("Something went wrong, Cypress could not prepare the retinal camera")
        );

        return;
    }

    ok = initializeDatabase();
    if (!ok)
    {
        QMessageBox::critical(m_parent, tr("Cypress"),
            tr("Something went wrong, Cypress could not initialize the retinal camera")
        );

        return;
    }
}

// SLOT
// actual measure will only execute if the barcode has been
// verified.  Subclasses must reimplement accordingly.
//
void RetinalCameraManager::measure()
{
    if (m_verbose)
    {
        qDebug() << "RetinalCameraManager::measure";
    }
}

// SLOT
// subclasses call methods just prior to main close event
//
void RetinalCameraManager::finish()
{
    if (m_verbose)
    {
        qDebug() << "RetinalCameraManager::finished";
    }

    // process data for left and right eye
    //
    QSqlQuery dataQuery;
    QSqlQuery locationQuery;

    bool ok = dataQuery.exec("SELECT FileName, FileExt, StoragePathUid FROM dbo.Media WHERE PatientUid = '" + m_defaultPatientUid + "'");
    if (!ok)
    {
        qDebug() << dataQuery.lastError();
        return;
    }

    while (dataQuery.next())
    {
        QString fileName = dataQuery.value(0).toString();
        QString fileExt = dataQuery.value(1).toString();
        QString storagePathUid = dataQuery.value(2).toString();

        ok = locationQuery.exec("SELECT Location FROM dbo.StoragePaths WHERE StoragePathUid = '" + m_defaultPatientUid + "'");

        if (!ok)
        {
            qDebug() << locationQuery.lastError();
            continue;
        }

        while (locationQuery.next())
        {
            // get image bytes and put into json
            // get eye side name and put into json

            QString location = locationQuery.value(0).toString();

            if (m_verbose)
            {
                qInfo() << "Located image: " << location << "/" << fileName + fileExt;
            }
        }

        if (m_verbose)
        {
            qDebug() << fileName << " " << fileExt << " " << storagePathUid;
        }
    }

    // save json data locally and send to Pine
    //
    emit complete(QJsonObject());
}

// SLOT
// Context dependent clear test data and possibly device data (eg., serial port info)
void RetinalCameraManager::clearData()
{
    if (m_verbose)
    {
        qDebug() << "RetinalCameraManager::clearData";
    }
}
