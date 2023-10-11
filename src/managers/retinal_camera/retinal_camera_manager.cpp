#include <QProcess>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QJsonDocument>
#include <QFileInfo>
#include <QSettings>
#include <QMessageBox>


#include "retinal_camera_manager.h"

#include "data/retinal_camera/retinal_camera_measurement.h"
#include "data/retinal_camera/database_manager.h"

#include "qsqlerror.h"
#include "cypress_session.h"

#include "auxiliary/file_utils.h"

RetinalCameraManager::RetinalCameraManager(const CypressSession& session):
    ManagerBase(session), m_test(new RetinalCameraTest)
{
}


RetinalCameraManager::~RetinalCameraManager()
{
    //finish();
}

bool RetinalCameraManager::isInstalled()
{
    return false;
}

bool RetinalCameraManager::isAvailable()
{
    return false;
}

void RetinalCameraManager::addManualMeasurement()
{
    QSharedPointer<RetinalCameraMeasurement> measurement(new RetinalCameraMeasurement);

    measurement->setAttribute("EYE_PICT_VENDOR", "");

    m_test->addMeasurement(measurement);

    emit dataChanged(m_test);
}

void RetinalCameraManager::start()
{
    emit started(m_test);
    emit canMeasure();

    //QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
    //QStringList arguments;

    //connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [this]
    //{
    //    measure();
    //    finish();
    //});

    //ok = startRetinalCamera();
    //if (!ok)
    //{
    //    qCritical() << "RetinalCameraManager:start() - could not start exe";
    //    return;
    //}
}

void RetinalCameraManager::measure()
{
    m_test->reset();
    m_test->simulate();

    emit measured(m_test);

    if (m_test->isValid())
    {
        emit canFinish();
    }
}

void RetinalCameraManager::finish()
{
    int answer_id = m_session.getAnswerId();

    for (int i = 0; i < m_test->getMeasurementCount(); i++)
    {
        Measurement& measure = m_test->get(i);
        const QString& side = measure.getAttribute("SIDE").toString();

        sendHTTPSRequest("PATCH", "https://blueberry.clsa-elcv.ca/qa/pine/api/answer/" + QString::number(answer_id) + "?filename=EYE_" + side + ".jpg", "application/octet-stream", FileUtils::readFileIntoByteArray(measure.getAttribute("EYE_PICT_VENDOR").toString()));
        measure.removeAttribute("EYE_PICT_VENDOR");
    }

    QJsonObject testJson = m_test->toJsonObject();

    testJson.insert("language", m_session.getLanguage());
    testJson.insert("session_id", m_session.getSessionId());
    testJson.insert("answer_id", m_session.getAnswerId());
    testJson.insert("barcode", m_session.getBarcode());
    testJson.insert("interviewer", m_session.getInterviewer());

    QJsonDocument jsonDoc(testJson);

    QByteArray serializedData = jsonDoc.toJson();

    sendHTTPSRequest("PATCH", "https://blueberry.clsa-elcv.ca/qa/pine/api/answer/" + QString::number(answer_id), "application/json", serializedData);

    emit success("sent");

    m_test->reset();
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



// Context dependent clear test data and possibly device data (eg., serial port info)
bool RetinalCameraManager::clearData()
{
    qDebug() << "RetinalCameraManager::clearData";
    return false;
}

void RetinalCameraManager::cancel()
{
    qDebug() << "RetinalCameraManager::cancel";
}

// set input parameters for the test
void RetinalCameraManager::setInputData(const QVariantMap& inputData)
{
    Q_UNUSED(inputData);
}

bool RetinalCameraManager::setUp()
{
    return true;
}

bool RetinalCameraManager::cleanUp()
{
   return true;
}

