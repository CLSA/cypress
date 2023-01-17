#include <QProcess>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QJsonDocument>
#include <QFileInfo>
#include <QSettings>
#include <QMessageBox>

#include "CypressApplication.h"
#include "auxiliary/JsonSettings.h"

#include "RetinalCameraManager.h"
#include "qsqlerror.h"


RetinalCameraManager::RetinalCameraManager(QWidget *parent)
    : ManagerBase{parent}
{
    QSettings qSettings;
}

// collate test results and device and other meta data
// for the main application to write to .json
//
bool RetinalCameraManager::backupDatabase()
{
    return false;
}

bool RetinalCameraManager::openDatabase()
{
    // Open DB connection and clean up database
    //QSqlDatabase db = QSqlDatabase::addDatabase(m_databaseDriver);
    //db.setHostName(m_databaseHost);
    //db.setPort(m_databasePort);
    //db.setDatabaseName(m_database);
    //db.setUserName(m_databaseUser);
    //db.setPassword(m_databasePassword);

    //bool ok = db.open();
    //if (!ok)
    //{
    //    qDebug() << "Could not open MS SQL database: " << db.lastError();
    //    printDatabaseParams();
    //}

    return true;
}

bool RetinalCameraManager::cleanupDatabase()
{
    // Remove data from relevant tables, and remove images from disk to prepare for a new test
    QSqlQuery query;
    QSqlQuery locationQuery;

    QString currentFileName;
    QString currentFileExtension;
    QString storagePathUid;

    //bool ok = query.exec("SELECT FileName, FileExt, StoragePathUid FROM dbo.Media WHERE PatientUid = '" + m_defaultPatientUid + "'");
    //if (!ok)
    //{
    //    qDebug() << query.lastQuery();
    //    qDebug() << query.lastError();

    //    return false;
    //}

    //while (query.next())
    //{
    //    currentFileName = query.value(0).toString();
    //    currentFileExtension = query.value(1).toString();
    //    storagePathUid = query.value(2).toString();

    //    ok = locationQuery.exec("SELECT Location FROM dbo.StoragePaths WHERE StoragePathUid = '" + storagePathUid + "'");
    //    if (!ok)
    //    {
    //        qDebug() << query.lastError();
    //        return false;
    //    }

    //    while (locationQuery.next())
    //    {
    //        QString location = locationQuery.value(0).toString();
    //        if (m_verbose)
    //        {
    //            qDebug() << "Deleting: " << location << "/" << currentFileName + currentFileExtension;
    //        }
    //    }

    //    if (m_verbose)
    //    {
    //        qDebug() << currentFileName << " " << currentFileExtension << " " << storagePathUid;
    //    }
    //}

    //ok = query.exec("DELETE FROM dbo.Exams WHERE PatientUid = '" + m_defaultPatientUid + "'");
    //if (!ok)
    //{
    //    qDebug() << query.lastError();
    //    return false;
    //}

    //ok = query.exec("DELETE FROM dbo.Media WHERE PatientUid = '" + m_defaultPatientUid + "'");
    //if (!ok) {
    //    qDebug() << query.lastError();
    //    return false;
    //}

    //ok = query.exec("DELETE FROM dbo.Patients WHERE PatientUid = '" + m_defaultPatientUid + "'");
    //if (!ok) {
    //    qDebug() << query.lastError();
    //    return false;
    //}

    //ok = query.exec("DELETE FROM dbo.Persons WHERE PersonUid = '" + m_defaultPatientUid + "'");
    //if (!ok) {
    //    qDebug() << query.lastError();
    //    return false;
    //}

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
    //m_process.setProgram(m_runnableName);
    //m_process.start();

    //bool started = m_process.waitForStarted();
    //if (!started)
    //{
    //    if (m_verbose)
    //        qDebug() << "RetinalCameraManager::startRetinalCamera: IMAGEnet_R4 did not start";
    //}

    //connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [this]
    //{
    //    finish();
    //});

    //return started;
    return true;
}

void getStoragePathQueryString()
{

}

void RetinalCameraManager::start()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
    //QFileInfo info(m_runnableName);
    QStringList arguments;

    bool ok = false;

    ok = openDatabase();
    if (!ok)
    {
        return;
    }

    ok = cleanupDatabase();
    if (!ok)
    {
        return;
    }

    ok = initializeDatabase();
    if (!ok)
    {
        return;
    }
}

// SLOT
// actual measure will only execute if the barcode has been
// verified.  Subclasses must reimplement accordingly.
//
void RetinalCameraManager::measure()
{
}

// SLOT
// subclasses call methods just prior to main close event
//
void RetinalCameraManager::finish()
{
    if (CypressApplication::mode == Mode::Sim)
    {
        QJsonObject results = JsonSettings::readJsonFromFile(
            "C:/work/clsa/cypress/src/tests/fixtures/retinal_camera/output.json"
        );
        if (results.empty()) return;

        bool ok = sendResultsToPine(results);
        if (!ok)
        {
            qDebug() << "Could not send results to Pine";
        }
    }
    // process data for left and right eye
    //
    //QSqlQuery dataQuery;
    //QSqlQuery locationQuery;

    //bool ok = dataQuery.exec("SELECT FileName, FileExt, StoragePathUid FROM dbo.Media WHERE PatientUid = '" + m_defaultPatientUid + "'");
    //if (!ok)
    //{
    //    qDebug() << dataQuery.lastError();
    //    return;
    //}

    //while (dataQuery.next())
    //{
    //    QString fileName = dataQuery.value(0).toString();
    //    QString fileExt = dataQuery.value(1).toString();
    //    QString storagePathUid = dataQuery.value(2).toString();

    //    ok = locationQuery.exec("SELECT Location FROM dbo.StoragePaths WHERE StoragePathUid = '" + m_defaultPatientUid + "'");

    //    if (!ok)
    //    {
    //        qDebug() << locationQuery.lastError();
    //        continue;
    //    }

    //    while (locationQuery.next())
    //    {
    //        // get image bytes and put into json
    //        // get eye side name and put into json

    //        QString location = locationQuery.value(0).toString();
    //        qInfo() << "Located image: " << location << "/" << fileName + fileExt;
    //    }
    //}

    //QJsonObject results = QJsonObject();
    //emit complete(results);
}

// SLOT
// Context dependent clear test data and possibly device data (eg., serial port info)
void RetinalCameraManager::clearData()
{
    qDebug() << "RetinalCameraManager::clearData";
}
