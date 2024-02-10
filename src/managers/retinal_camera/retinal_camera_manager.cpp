#include "retinal_camera_manager.h"
#include "auxiliary/network_utils.h"

#include "cypress_session.h"
#include "cypress_settings.h"

#include "data/retinal_camera/database_manager.h"
#include "data/retinal_camera/retinal_camera_test.h"

#include "auxiliary/file_utils.h"

#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

RetinalCameraManager::RetinalCameraManager(QSharedPointer<RetinalCameraSession> session)
    : ManagerBase(session)
{
    m_runnableName = CypressSettings::readSetting("retinal_camera/runnableName").toString();
    m_runnablePath = CypressSettings::readSetting("retinal_camera/runnablePath").toString();

    m_databaseName = CypressSettings::readSetting("retinal_camera/database/name").toString();
    m_databasePort = CypressSettings::readSetting("retinal_camera/database/port").toInt();
    m_databaseUser = CypressSettings::readSetting("retinal_camera/database/user").toString();
    m_databasePassword = CypressSettings::readSetting("retinal_camera/database/password").toString();

    m_test.reset(new RetinalCameraTest);
    m_test->setExpectedMeasurementCount(1); // one cypress session per eye
}

RetinalCameraManager::~RetinalCameraManager()
{
    if (m_db.isOpen())
        m_db.close();
}

bool RetinalCameraManager::isInstalled()
{
    bool isDebugMode = CypressSettings::isDebugMode();
    QString runnableName = CypressSettings::readSetting("retinal_camera/runnableName").toString();
    QString runnablePath = CypressSettings::readSetting("retinal_camera/runnablePath").toString();

    QString databaseName = CypressSettings::readSetting("retinal_camera/database/name").toString();
    QString databasePort = CypressSettings::readSetting("retinal_camera/database/port").toString();
    QString databaseUser = CypressSettings::readSetting("retinal_camera/database/user").toString();

    if (runnableName.isEmpty() || runnablePath.isNull()) {
        if (isDebugMode)
            qDebug() << "RetinalCamera: runnable name is not defined";
        return false;
    }

    if (runnablePath.isEmpty() || runnablePath.isNull()) {
        if (isDebugMode)
            qDebug() << "RetinalCamera: runnable path is not defined";
        return false;
    }

    if (databaseName.isEmpty() || databaseName.isNull()) {
        if (isDebugMode)
            qDebug() << "RetinalCamera: database name is not defined";
        return false;
    }

    if (databasePort.isEmpty() || databasePort.isNull()) {
        if (isDebugMode)
            qDebug() << "RetinalCamera: database port is not defined";
        return false;
    }

    QFileInfo runnableNameInfo(runnableName);
    QDir runnablePathInfo(runnablePath);

    if (!runnableNameInfo.exists() || !runnableNameInfo.isExecutable()) {
        if (isDebugMode)
            qDebug() << "RetinalCamera: ImageNet exe is not defined";
        return false;
    }

    if (!runnablePathInfo.exists()) {
        if (isDebugMode)
            qDebug() << "RetinalCamera: working directory does not exist";
        return false;
    }

    return true;
}

bool RetinalCameraManager::start()
{
    if (m_debug)
        qDebug() << "RetinalCameraManager::start";

    if (m_sim) {
        emit started(m_test);
        emit canMeasure();
        emit dataChanged(m_test);

        return true;
    }

    if (!openDatabase()) {
        emit error("Could not open IMAGENet database");
        return false;
    }

    if (!setUp()) {
        emit error("Could not setup IMAGENet database");
        return false;
    }

    measure();

    return true;
}

bool RetinalCameraManager::openDatabase()
{
    if (m_debug) {
        qDebug() << "RetinalCameraManager::openDatabase";
    }

    m_db = QSqlDatabase::addDatabase("QODBC");
    m_db.setHostName("localhost");
    m_db.setPort(m_databasePort);
    m_db.setDatabaseName(m_databaseName);
    m_db.setUserName(m_databaseUser);
    m_db.setPassword(m_databasePassword);

    if (!m_db.open()) {
        qDebug() << "RetinalCameraManager::openDatabase - could not open database: "
                 << m_db.lastError();

        return false;
    }

    return true;
}

bool RetinalCameraManager::setUp()
{
    if (m_debug)
        qDebug() << "RetinalCameraManager::setUp";

    if (!m_db.isOpen()) {
        qDebug() << "IMAGENet database is not open";
        return false;
    }

    if (!cleanUp()) {
        if (m_debug)
            qDebug() << "RetinalCameraManager::setUp cleanup did not return true";
        return false;
    }

    return initializeDatabase();
}

bool RetinalCameraManager::cleanUp()
{
    if (m_debug)
        qDebug() << "RetinalCameraManager::cleanUp";

    if (!m_db.isOpen())
        return false;

    return cleanupDatabase();
}

void RetinalCameraManager::configureProcess()
{
    if (m_debug)
        qDebug() << "RetinalCameraManager::configureProcess";

    // connect signals and slots to QProcess one time only
    //
    connect(&m_process, &QProcess::started, this, [this]() {
        if (m_debug)
            qDebug() << "RetinalCameraManager::process started: "
                     << m_process.arguments().join(" ");
    });

    // read output from csv after process finishes
    connect(&m_process,
            QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this,
            &RetinalCameraManager::readOutput);

    // error occured,
    connect(&m_process, &QProcess::errorOccurred, this, [=](QProcess::ProcessError error) {
        QStringList s = QVariant::fromValue(error).toString().split(QRegExp("(?=[A-Z])"),
                                                                    Qt::SkipEmptyParts);
        if (m_debug)
            qDebug() << "ERROR: process error occured: " << s.join(" ").toLower();
    });

    connect(&m_process, &QProcess::stateChanged, this, [=](QProcess::ProcessState state) {
        QStringList s = QVariant::fromValue(state).toString().split(QRegExp("(?=[A-Z])"),
                                                                    Qt::SkipEmptyParts);
        if (m_debug)
            qDebug() << "process state: " << s.join(" ").toLower();
    });

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");

    m_process.setProgram(settings.value("retinal_camera/working_dir").toString()
                         + settings.value("retinal_camera/executable").toString());
}

void RetinalCameraManager::measure()
{
    if (m_debug)
        qDebug() << "RetinalCameraManager::measure";

    if (m_sim) {
        RetinalCameraSession &session = static_cast<RetinalCameraSession &>(*m_session);
        m_test->simulate({{"side", session.getSide() == Side::Left ? "left" : "right"}});

        emit dataChanged(m_test);
        emit canFinish();

        return;
    }

    if (m_process.state() != QProcess::NotRunning) {
        QMessageBox::critical(nullptr, "Error", "Program is already running");
        return;
    }

    clearData();

    m_process.start();
    if (!m_process.waitForStarted()) {
        QMessageBox::critical(nullptr, "Error", "Could not launch program");
        return;
    }
}

void RetinalCameraManager::finish()
{
    if (m_debug)
        qDebug() << "RetinalCameraManager::finish";

    int answer_id = m_session->getAnswerId();

    QJsonObject testJson = m_test->toJsonObject();
    testJson.insert("session", m_session->getJsonObject());

    for (int i = 0; i < m_test->getMeasurementCount(); i++) {
        Measurement& measure = m_test->get(i);
        const QString &side = measure.getAttribute("EYE_SIDE_VENDOR").toString();

        QString host = CypressSettings::getPineHost();
        QString endpoint = CypressSettings::getPineEndpoint();

        QString fileName = "EYE_" + side;

        testJson.insert("files", QJsonObject {{ fileName, FileUtils::getHumanReadableFileSize(measure.getAttribute("EYE_PICT_VENDOR").toString())}});

        bool ok = NetworkUtils::sendHTTPSRequest("PATCH",
                                   (host + endpoint + QString::number(answer_id) + "?filename=EYE_"
                                                  + side + ".jpg").toStdString(),
                                   "application/octet-stream",
                                   FileUtils::readFile(
                                       measure.getAttribute("EYE_PICT_VENDOR").toString()));
        if (!ok) {
        }

        measure.removeAttribute("EYE_PICT_VENDOR");
    }

    QJsonObject values;
    values.insert("value", testJson);

    QJsonDocument jsonDoc(values);
    QByteArray serializedData = jsonDoc.toJson();

    QString answerUrl = CypressSettings::getAnswerUrl(answer_id);
    bool ok = NetworkUtils::sendHTTPSRequest("PATCH", answerUrl.toStdString(), "application/json", serializedData);

    if (ok) {
        emit success("Save successful. You may close this window.");
    }
    else {
        emit error("Could not save results, please contact support.");
    }

}

bool RetinalCameraManager::cleanupDatabase()
{
    if (m_debug)
        qDebug() << "RetinalCameraManager::cleanupDatabase";

    if (!m_db.open()) {
        if (m_debug) {
            qDebug() << "could not open database";
        }

        return false;
    }

    QSqlQuery query(m_db);
    query.prepare("SELECT FileName, FileExt, StoragePathUid FROM dbo.Media WHERE PatientUid = :patientUUID");
    query.bindValue(":patientUUID", defaultPatientUUID);
    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return false;
    }

    while (query.next())
    {
        QString storagePathUid = query.value("StoragePathUid").toString();
        QString fileName = query.value("FileName").toString().trimmed();
        QString extension = query.value("FileExt").toString().trimmed();
        QString location = EyeExtractorQueryUtil::getLocation(m_db, storagePathUid);

        if (m_debug) {
            qDebug() << "RetinalCameraManager::cleanupDatabase - deleting:"
                     << location + "/" + fileName + extension;
        }

        QFile file(location + "/" + fileName + extension);
        file.remove();
    }

    query.prepare("DELETE FROM dbo.Exams WHERE PatientUid = :patientUUID");
    query.bindValue(":patientUUID", defaultPatientUUID);
    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return false;
    }


    query.prepare("DELETE FROM dbo.Media WHERE PatientUid = :patientUUID");
    query.bindValue(":patientUUID", defaultPatientUUID);
    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return false;
    }

    query.prepare("DELETE FROM dbo.Patients WHERE PatientUid = :patientUUID");
    query.bindValue(":patientUUID", defaultPatientUUID);
    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return false;
    }

    query.prepare("DELETE FROM dbo.Persons WHERE PersonUid = :personUUID");
    query.bindValue(":personUUID", defaultPatientUUID);
    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return false;
    }

    return true;
}

bool RetinalCameraManager::initializeDatabase()
{
    if (m_debug)
        qDebug() << "RetinalCameraManager::initializeDatabase";

    QString participantId = m_session->getBarcode();
    QSqlQuery query(m_db);

    query.prepare("INSERT INTO dbo.Persons (PersonUid, SurName, ForeName) VALUES (:personUUID, "
                  ":lastName, :firstName)");
    query.bindValue(":personUUID", defaultPersonUUID);
    query.bindValue(":lastName", "Participant");
    query.bindValue(":firstName", "CLSA");
    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return false;
    }

    query.prepare("INSERT INTO dbo.Patients (PatientUid, PatientIdentifier, PersonUid) VALUES (:patientUUID, :participantId, :personUUID)");
    query.bindValue(":patientUUID", defaultPatientUUID);
    query.bindValue(":participantId", participantId);
    query.bindValue(":personUUID", defaultPatientUUID);
    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return false;
    }

    return true;
}

bool RetinalCameraManager::clearData()
{
    if (m_debug)
        qDebug() << "RetinalCameraManager::clearData";

    m_test->reset();
    emit dataChanged(m_test);

    return false;
}

void RetinalCameraManager::readOutput()
{
    if (QProcess::NormalExit != m_process.exitStatus()) {
        emit error("Process failed to finish correctly, cannot read output");
        return;
    }

    QSharedPointer<RetinalCameraSession> session = qSharedPointerCast<RetinalCameraSession>(m_session);
    QSharedPointer<RetinalCameraTest> test = qSharedPointerCast<RetinalCameraTest>(m_test);

    QJsonObject results;
    switch (session->getSide()) {
        case Side::Left:
            results = getLeftEye();
            break;
        case Side::Right:
            results = getRightEye();
            break;
    }

    test->fromDatabaseResults(results);

    finish();
}

QJsonObject RetinalCameraManager::getLeftEye()
{
    QSqlQuery query(m_db);
    query.prepare(
        "SELECT FileName, FileExt, StoragePathUid, CreateDate FROM dbo.Media WHERE PatientUid = "
        ":patientUid AND EyeType = 1 AND Status = 1 AND Display = 1 ORDER BY CreateDate ASC");
    query.bindValue(":patientUid", defaultPatientUUID);
    if (!query.exec()) {
        qDebug() << query.lastError().text();
    }

    QJsonObject results;
    results["fileName"] = query.value(0).toString();
    results["fileExt"] = query.value(1).toString();
    results["side"] = "left";
    results["storagePathUid"] = query.value(2).toString();
    results["createDate"] = query.value(3).toString();

    query.prepare("SELECT Location FROM dbo.StoragePaths WHERE StoragePathUid = :storagePathUid");
    query.bindValue(":storagePathUid", results["storagePathUid"].toString());

    if (!query.exec()) {
        qDebug() << query.lastError().text();
    }

    results["filePath"] = query.value(0).toString();

    return results;
}

QJsonObject RetinalCameraManager::getRightEye()
{
    QSqlQuery query(m_db);
    query.prepare(
        "SELECT FileName, FileExt, StoragePathUid, CreateDate FROM dbo.Media WHERE PatientUid = "
        ":patientUid AND EyeType = 2 AND Status = 1 AND Display = 1 ORDER BY CreateDate ASC");
    query.bindValue(":patientUid", defaultPatientUUID);

    if (!query.exec()) {
        qDebug() << query.lastError().text();
    }

    QJsonObject results;
    results["fileName"] = query.value(0).toString();
    results["fileExt"] = query.value(1).toString();
    results["side"] = "right";
    results["storagePathUid"] = query.value(2).toString();
    results["createDate"] = query.value(3).toString();

    query.prepare("SELECT Location FROM dbo.StoragePaths WHERE StoragePathUid = :storagePathUid");
    query.bindValue(":storagePathUid", results["storagePathUid"].toString());

    if (!query.exec()) {
        qDebug() << query.lastError().text();
    }

    results["filePath"] = query.value(0).toString();

    return results;
}
