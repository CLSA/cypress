#include "retinal_camera_manager.h"
#include "cypress_application.h"

#include "data/retinal_camera/retinal_camera_test.h"
#include "data/retinal_camera/retinal_camera_measurement.h"

#include "data/retinal_camera/database_manager.h"

#include "auxiliary/Utilities.h"
#include "cypress_settings.h"

#include "qsqlerror.h"
#include "cypress_session.h"
#include "auxiliary/file_utils.h"

#include <QProcess>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QJsonDocument>
#include <QFileInfo>
#include <QSettings>
#include <QMessageBox>
#include <QJsonObject>


RetinalCameraManager::RetinalCameraManager(QSharedPointer<RetinalCameraSession> session)
    : ManagerBase(session)
{
    m_test.reset(new RetinalCameraTest);
}


RetinalCameraManager::~RetinalCameraManager()
{
    //finish();
}

bool RetinalCameraManager::isInstalled()
{
    return false;
}

void RetinalCameraManager::start()
{
    if (m_debug)
    {
        qDebug() << "RetinalCameraManager::start";
    }


    configureProcess();

    m_process.start();

    emit started(m_test.get());

    if (!startRetinalCamera())
    {
        qCritical() << "RetinalCameraManager:start() - could not start exe";
        return;
    }
}

void RetinalCameraManager::measure()
{
    if (m_debug)
    {
        qDebug() << "RetinalCameraManager::measure";
    }

    RetinalCameraSession& session = dynamic_cast<RetinalCameraSession&>(*m_session);

    m_test->reset();

    if (m_sim)
    {
        m_test->simulate({{"side", session.getSide() == Side::Left ? "left" : "right"}});
        emit measured(m_test.get());
        emit canFinish();
    }
}

void RetinalCameraManager::finish()
{
    if (m_debug)
    {
        qDebug() << "RetinalCameraManager::finish";
    }

    int answer_id = m_session->getAnswerId();

    QJsonObject testJson = m_test->toJsonObject();
    testJson.insert("session", m_session->getJsonObject());

    //side = session
    for (int i = 0; i < m_test->getMeasurementCount(); i++) {
        Measurement& measure = m_test->get(i);
        const QString &side = measure.getAttribute("EYE_SIDE_VENDOR").toString();

        QString host = CypressSettings::getPineHost();
        QString endpoint = CypressSettings::getPineEndpoint();

        QByteArray fileContents = FileUtils::readFile(measure.getAttribute("EYE_PICT_VENDOR").toString());
        QString fileName = "EYE_" + side;

        testJson.insert("files", QJsonObject {{ fileName, FileUtils::getHumanReadableFileSize(measure.getAttribute("EYE_PICT_VENDOR").toString())}});

        sendHTTPSRequest("PATCH",
                         host + endpoint + QString::number(answer_id) + "?filename=EYE_" + side
                             + ".jpg",
                         "application/octet-stream",
                         FileUtils::readFile(
                             measure.getAttribute("EYE_PICT_VENDOR").toString()));

        measure.removeAttribute("EYE_PICT_VENDOR");
    }

    QJsonObject values {};
    values.insert("value", testJson);

    QJsonDocument jsonDoc(values);
    QByteArray serializedData = jsonDoc.toJson();

    QString answerUrl = CypressSettings::getAnswerUrl(answer_id);
    sendHTTPSRequest("PATCH",
                     answerUrl,
                     "application/json",
                     serializedData);

    emit success("sent");
}

bool RetinalCameraManager::backupDatabase()
{
    if (m_debug)
    {
        qDebug() << "RetinalCameraManager::backupDatabase";
    }

    return false;
}

bool RetinalCameraManager::openDatabase()
{
    if (m_debug)
    {
        qDebug() << "RetinalCameraManager::openDatabase";
    }

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

bool RetinalCameraManager::cleanupDatabase()
{
    if (m_debug)
    {
        qDebug() << "RetinalCameraManager::cleanupDatabase";
    }

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

bool RetinalCameraManager::restoreDatabase()
{
    if (m_debug)
    {
        qDebug() << "RetinalCameraManager::restoreDatabase";
    }

    return false;
}

bool RetinalCameraManager::initializeDatabase()
{
    if (m_debug)
    {
        qDebug() << "RetinalCameraManager::initializeDatabase";
    }

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

    return true;
}

bool RetinalCameraManager::startRetinalCamera()
{
    if (m_debug)
    {
        qDebug() << "RetinalCameraManager::startRetinalCamera";
    }

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

void RetinalCameraManager::configureProcess()
{
    if (m_debug)
    {
        qDebug() << "RetinalCameraManager::configureProcess";
    }
}

bool RetinalCameraManager::clearData()
{
    if (m_debug)
    {
        qDebug() << "RetinalCameraManager::clearData";
    }

    m_test->reset();
    emit dataChanged(m_test.get());

    return false;
}

void RetinalCameraManager::cancel()
{
    if (m_debug)
    {
        qDebug() << "RetinalCameraManager::cancel";
    }
}

bool RetinalCameraManager::setUp()
{
    if (m_debug)
    {
        qDebug() << "RetinalCameraManager::setUp";
    }

    return true;
}

bool RetinalCameraManager::cleanUp()
{
    if (m_debug)
    {
        qDebug() << "RetinalCameraManager::cleanUp";
    }

    return true;
}

