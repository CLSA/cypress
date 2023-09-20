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
    RetinalCameraMeasurement measurement;

    measurement.setAttribute("EYE_PICT_VENDOR", "SIM");

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
    //if (Cypress::getInstance().isSimulation()) {
    m_test->reset();
    m_test->simulate();
    //}
    // else {
    //bool ok = false;
    //ok = openDatabase();
    //if (!ok)
    //{
    //    qCritical() << "RetinalCameraManager:start() - could not open database";
    //    return;
    //}

    //ok = cleanupDatabase();
    //if (!ok)
    //{
    //    qCritical() << "RetinalCameraManager:start() - could not cleanup database";
    //    return;
    //}

    //ok = initializeDatabase();
    //if (!ok)
    //{
    //    qCritical() << "RetinalCameraManager:start() - could not initialize database";
    //    return;
    //}
    //}

    emit measured(m_test);

    if (m_test->isValid())
    {
        emit canFinish();
    }

    //}

    //qDebug() << "RetinalCameraManager::measure";

    //QMap<QString, QVariant> leftData = EyeExtractorQueryUtil::extractData(m_db, defaultPatientUUID, 1, "Left");
    //qDebug() << "RetinalCameraManager::finish - left data: " << leftData;

    //QMap<QString, QVariant> rightData = EyeExtractorQueryUtil::extractData(m_db, defaultPatientUUID, 2, "Right");
    //qDebug() << "RetinalCameraManager::finish - right data: " << rightData;

    //QJsonObject response {
    //    { "cypress_session", m_uuid },
    //    { "answer_id", m_answerId },
    //};

    //response["cypress_session"] = m_uuid;
    //response["answer_id"] = m_answerId;
    //response["barcode"] = m_barcode;
    //response["interviewer"] = m_interviewer;

    //sendResultsToPine(response);

    //if (!leftData["EYE_PICT_VENDOR"].isNull())
    //{
    //    sendFileToPine(leftData["EYE_PICT_VENDOR"].toByteArray(), "left_eye.jpeg");
    //}
    //if (!rightData["EYE_PICT_VENDOR"].isNull())
    //{
    //    sendFileToPine(rightData["EYE_PICT_VENDOR"].toByteArray(), "right_eye.jpeg");
    //}
}

void RetinalCameraManager::finish()
{
    emit success("sent");

    //emit error("Test");
    //emit sent();
    //cleanupDatabase();
    //m_db.close();

    //sendResultsToPine("C:/dev/clsa/cypress/src/tests/fixtures/retinal_camera/output.json");
    //sendFileToPine("C:/dev/clsa/cypress/src/tests/fixtures/retinal_camera/left.jpg", "left.jpg");
    //sendFileToPine("C:/dev/clsa/cypress/src/tests/fixtures/retinal_camera/right.jpg", "right.jpg");
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

