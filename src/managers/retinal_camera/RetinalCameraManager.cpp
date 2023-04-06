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

RetinalCameraManager::RetinalCameraManager(QString uuid): m_uuid { uuid }
{

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

    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [this]
    {
        measure();
        finish();
    });

    ok = startRetinalCamera();
    if (!ok)
    {
        qCritical() << "RetinalCameraManager:start() - could not start exe";
        return;
    }
}

bool RetinalCameraManager::startRetinalCamera()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");

    m_process.setProgram(settings.value("instruments/retinal_camera/exe").toString());
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
    QMap<QString, QVariant> leftData = EyeExtractorQueryUtil::extractData(m_db, defaultPatientUUID, 1, "Left");
    QMap<QString, QVariant> rightData = EyeExtractorQueryUtil::extractData(m_db, defaultPatientUUID, 2, "Right");

    QJsonObject leftEyeObject;
    for (const QString &key : leftData.keys())
    {
        leftEyeObject.insert(key, QJsonValue::fromVariant(leftData.value(key)));
    }

    QJsonObject rightEyeObject;
    for (const QString &key : rightData.keys())
    {
        rightEyeObject.insert(key, QJsonValue::fromVariant(rightData.value(key)));
    }

    QJsonObject examJson {
        { "left" , leftEyeObject },
        { "right", rightEyeObject }
    };

    qDebug().noquote() << m_exam.toJson(QJsonDocument::Indented);

    m_exam = QJsonDocument(examJson);
}

void RetinalCameraManager::finish()
{
    cleanupDatabase();
    m_db.close();
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
    m_db = QSqlDatabase::addDatabase(settings.value("instruments/retinal_camera/database/driver").toString());
    m_db.setHostName(settings.value("instruments/retinal_camera/database/host").toString());
    m_db.setPort(settings.value("instruments/retinal_camera/database/port").toInt());
    m_db.setDatabaseName(settings.value("instruments/retinal_camera/database/database").toString());
    m_db.setUserName(settings.value("instruments/retinal_camera/database/username").toString());
    m_db.setPassword(settings.value("instruments/retinal_camera/database/password").toString());

    bool ok = m_db.open();
    if (!ok)
    {
        qDebug() << "Could not open database: " << m_db.lastError();
        return false;
        //printDatabaseParams();
    }

    return true;
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
    qDebug() << query.lastError();
    qDebug() << query.lastInsertId();

    query.prepare("INSERT INTO dbo.Patients (PatientUid, PatientIdentifier, PersonUid) VALUES (:patientUUID, :participantId, :personUUID)");
    query.bindValue(":patientUUID", defaultPatientUUID);
    query.bindValue(":participantId", participantId);
    query.bindValue(":personUUID", defaultPatientUUID);
    query.exec();
    qDebug() << query.lastError();

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
        QString location = EyeExtractorQueryUtil::getLocation(m_db, storagePathUid);
        qDebug() << "RetinalCameraManager::cleanupDatabase - deleting:" << location + "/" + fileName + extension;
        QFile file(location + "/" + fileName + extension);
        file.remove();
    }

    query.prepare("DELETE FROM dbo.Exams WHERE PatientUid = :patientUUID");
    query.bindValue(":patientUUID", defaultPatientUUID);
    query.exec();
    qDebug() << query.lastError();

    query.prepare("DELETE FROM dbo.Media WHERE PatientUid = :patientUUID");
    query.bindValue(":patientUUID", defaultPatientUUID);
    query.exec();
    qDebug() << query.lastError();

    query.prepare("DELETE FROM dbo.Patients WHERE PatientUid = :patientUUID");
    query.bindValue(":patientUUID", defaultPatientUUID);
    query.exec();
    qDebug() << query.lastError();

    query.prepare("DELETE FROM dbo.Persons WHERE PersonUid = :personUUID");
    query.bindValue(":personUUID", defaultPatientUUID);
    query.exec();
    qDebug() << query.lastError();

    return true;
}


