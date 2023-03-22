#include <QProcess>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QJsonDocument>
#include <QFileInfo>
#include <QSettings>
#include <QMessageBox>

#include "RetinalCameraManager.h"
#include "qsqlerror.h"

#include "../../data/retinal_camera/databasemanager.h"

RetinalCameraManager::RetinalCameraManager(QString uuid)
    : ManagerBase {}, m_uuid { uuid }
{
}


bool RetinalCameraManager::isAvailable()
{
    return false;
}

void RetinalCameraManager::start()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
    QStringList arguments;

    bool ok = false;
    ok = openDatabase();
    if (!ok)
    {
        qCritical() << "RetinalCameraManager:start() - could not open database";
        return;
    }

    ok = cleanupDatabase();
    if (!ok)
    {
        qCritical() << "RetinalCameraManager:start() - could not cleanup database";
        return;
    }

    ok = initializeDatabase();
    if (!ok)
    {
        qCritical() << "RetinalCameraManager:start() - could not initialize database";
        return;
    }

    ok = startRetinalCamera();
    if (!ok)
    {
        qCritical() << "RetinalCameraManager:start() - could not start exe";
        return;
    }

    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [this]
    {
        finish();
    });
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
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
    m_db = QSqlDatabase::addDatabase(settings.value("retinal_camera/database/driver").toString());

    m_db.setHostName(settings.value("retinal_camera/database/host").toString());
    m_db.setPort(settings.value("retinal_camera/database/port").toInt());
    m_db.setDatabaseName(settings.value("retinal_camera/database/port").toString());
    m_db.setUserName(settings.value("retinal_camera/database/username").toString());
    m_db.setPassword(settings.value("retinal_camera/database/password").toString());

    bool ok = m_db.open();
    if (!ok)
    {
        qDebug() << "Could not open database: " << m_db.lastError();
        return false;
        //printDatabaseParams();
    }

    return true;
}

bool RetinalCameraManager::cleanupDatabase()
{
    qDebug() << "RetinalCameraManager:: cleaning data";

    QSqlQuery query(m_db);
    query.prepare("SELECT FileName, FileExt, StoragePathUid FROM dbo.Media WHERE PatientUid = :patientUUID");
    query.bindValue(":patientUUID", defaultPatientUUID);
    query.exec();

    while (query.next())
    {
        QString storagePathUid = query.value("StoragePathUid").toString();
        QString fileName = query.value("FileName").toString().trimmed();
        QString extension = query.value("FileExt").toString().trimmed();
        // Assuming EyeExtractorQueryUtil::getLocation() is a function that takes a QSqlDatabase object and a QString object as its arguments
        QString location = EyeExtractorQueryUtil::getLocation(m_db, storagePathUid);
        qDebug() << "RetinalCameraManager::cleanupDatabase - deleting:" << location + "/" + fileName + extension;
        QFile file(location + "/" + fileName + extension);
        file.remove();
    }

    query.prepare("DELETE FROM dbo.Exams WHERE PatientUid = :patientUUID");
    query.bindValue(":patientUUID", defaultPatientUUID);
    query.exec();

    query.prepare("DELETE FROM dbo.Media WHERE PatientUid = :patientUUID");
    query.bindValue(":patientUUID", defaultPatientUUID);
    query.exec();

    query.prepare("DELETE FROM dbo.Patients WHERE PatientUid = :patientUUID");
    query.bindValue(":patientUUID", defaultPatientUUID);
    query.exec();

    query.prepare("DELETE FROM dbo.Persons WHERE PersonUid = :personUUID");
    query.bindValue(":personUUID", defaultPatientUUID);
    query.exec();

    return true;
}

bool RetinalCameraManager::restoreDatabase()
{
    return false;
}

bool RetinalCameraManager::initializeDatabase()
{
    QString participantId = "123456789";

    QSqlQuery query(m_db);
    query.prepare("INSERT INTO dbo.Persons (PersonUid, SurName, ForeName) VALUES (:personUUID, :lastName, :firstName)");
    query.bindValue(":personUUID", defaultPersonUUID);
    query.bindValue(":lastName", "Study");
    query.bindValue(":firstName", "Participant");
    query.exec();

    query.prepare("INSERT INTO dbo.Patients (PatientUid, PatientIdentifier, PersonUid) VALUES (:patientUUID, :participantId, :personUUID)");
    query.bindValue(":patientUUID", defaultPatientUUID);
    query.bindValue(":participantId", participantId);
    query.bindValue(":personUUID", defaultPatientUUID);
    query.exec();

    //QSqlQuery query;
    //bool ok = query.exec("INSERT INTO dbo.Persons (PersonUid, SurName, ForeName) values('11111111-2222-3333-4444-555555555555', 'Study', 'Participant')");
    //if (!ok)
    //{
    //    qDebug() << query.lastError();
    //    return false;
    //}

    //ok = query.exec("INSERT INTO dbo.Patients (PatientUid, PatientIdentifier, PersonUid) values('11111111-2222-3333-4444-555555555555', '11111111-2222-3333-4444-555555555555', '11111111-2222-3333-4444-555555555555')");
    //if (!ok)
    //{
    //    qDebug() << query.lastError();
    //    return false;
    //}

    return true;
}

bool RetinalCameraManager::startRetinalCamera()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");

    m_process.setProgram(settings.value("retinal_camera/working_dir").toString() + settings.value("retinal_camera/executable").toString());
    m_process.start();

    bool started = m_process.waitForStarted();
    if (!started)
    {
        qDebug() << "RetinalCameraManager::startRetinalCamera: IMAGEnet_R4 did not start";
    }

    return started;
}



void RetinalCameraManager::measure()
{
    qDebug() << "RetinalCameraManager::measure";
}

void RetinalCameraManager::finish()
{
    qDebug() << "RetinalCameraManager::finish()";

    // Assuming LeftEyeExtractor and RightEyeExtractor are implemented
    QMap<QString, QVariant> leftData = EyeExtractorQueryUtil::extractData(m_db, defaultPatientUUID, 1, "Left");
    //if (leftData.contains(EYE_PICT_VENDOR))
    //{
    //    qDebug() << "Left Data found";
    //    m_instrumentExecutionService.addOutputParameterValues(leftData);
    //}

    QMap<QString, QVariant> rightData = EyeExtractorQueryUtil::extractData(m_db, defaultPatientUUID, 2, "Right");
    //if (rightData.contains(EyeExtractor::EYE_PICT_VENDOR))
    //{
    //    qDebug() << "Right Data found";
    //    m_instrumentExecutionService.addOutputParameterValues(rightData);
    //}

    qDebug() << "RetinalCameraManager::finish - left data: " << leftData;
    qDebug() << "RetinalCameraManager::finish - right data: " << rightData;


    cleanupDatabase();
    m_db.close();
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

// Context dependent clear test data and possibly device data (eg., serial port info)
bool RetinalCameraManager::clearData()
{
    qDebug() << "RetinalCameraManager::clearData";
    return false;
}

// set input parameters for the test
void RetinalCameraManager::setInputData(const QVariantMap& inputData)
{

}

bool RetinalCameraManager::setUp()
{
    return true;
}

bool RetinalCameraManager::cleanUp()
{
   return true;
}

