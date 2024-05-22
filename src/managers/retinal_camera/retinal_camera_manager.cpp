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
}

bool RetinalCameraManager::isInstalled()
{
    const QString runnableName = CypressSettings::readSetting("retinal_camera/runnableName").toString();
    const QString runnablePath = CypressSettings::readSetting("retinal_camera/runnablePath").toString();

    const QString databaseName = CypressSettings::readSetting("retinal_camera/database/name").toString();
    const QString databasePort = CypressSettings::readSetting("retinal_camera/database/port").toString();
    const QString databaseUser = CypressSettings::readSetting("retinal_camera/database/user").toString();

    if (runnableName.isEmpty() || runnableName.isNull()) {
        qInfo() << "RetinalCamera: runnable name is not defined";
        return false;
    }

    if (runnablePath.isEmpty() || runnablePath.isNull()) {
        qInfo() << "RetinalCamera: runnable path is not defined";
        return false;
    }

    if (databaseName.isEmpty() || databaseName.isNull()) {
        qInfo() << "RetinalCamera: database name is not defined";
        return false;
    }

    if (databasePort.isEmpty() || databasePort.isNull()) {
        qInfo() << "RetinalCamera: database port is not defined";
        return false;
    }

    const QFileInfo runnableNameInfo(runnableName);
    const QDir runnablePathInfo(runnablePath);

    if (!runnableNameInfo.exists() || !runnableNameInfo.isExecutable()) {
        qInfo() << "RetinalCamera: ImageNet exe is not defined";
        return false;
    }

    if (!runnablePathInfo.exists()) {
        qInfo() << "RetinalCamera: working directory does not exist";
        return false;
    }

    return true;
}

bool RetinalCameraManager::start()
{
    qInfo() << "RetinalCameraManager::start";

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
    qInfo() << "RetinalCameraManager::openDatabase";

    m_db = QSqlDatabase::addDatabase("QODBC");
    m_db.setDatabaseName("IMAGENet");

    if (!m_db.open()) {
        qCritical() << "RetinalCameraManager::openDatabase:" << m_db.lastError();

        return false;
    }

    return true;
}

bool RetinalCameraManager::setUp()
{
    qInfo() << "RetinalCameraManager::setUp";

    if (!m_db.isOpen()) {
        if (!m_db.open()) {
            qCritical() << "RetinalCameraManager::setUp: could not open database";
            return false;
        }
    }

    if (!cleanUp()) {
        qCritical() << "RetinalCameraManager::setUp: cleanup did not succeed";
        return false;
    }

    configureProcess();

    if (!initializeDatabase())
        return false;

    return true;
}

bool RetinalCameraManager::cleanUp()
{
    qInfo() << "RetinalCameraManager::cleanUp";

    if (QProcess::NotRunning != m_process.state())
        m_process.close();

    if (!m_db.isOpen())
        return false;

    return cleanupDatabase();
}

void RetinalCameraManager::configureProcess()
{
    qInfo() << "RetinalCameraManager::configureProcess";

    // connect signals and slots to QProcess one time only
    //
    connect(&m_process, &QProcess::started, this, [this]() {
        qInfo() << "RetinalCameraManager::process started: " << m_process.arguments().join(" ");
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
        qWarning() << "RetinalCameraManager::configureProcess: process error occured: " << s.join(" ").toLower();
    });

    connect(&m_process, &QProcess::stateChanged, this, [=](QProcess::ProcessState state) {
        QStringList s = QVariant::fromValue(state).toString().split(QRegExp("(?=[A-Z])"),
                                                                    Qt::SkipEmptyParts);
        qInfo() << "RetinalCameraManager::configureProcess: process state: " << s.join(" ").toLower();
    });

    m_process.setProgram(m_runnableName);
}

void RetinalCameraManager::measure()
{
    qInfo() << "RetinalCameraManager::measure";

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
    qInfo() << "RetinalCameraManager::finish";

    const int answer_id = m_session->getAnswerId();
    const QString pineOrigin = m_session->getOrigin();
    const QString answerUrl = pineOrigin + "/answer/" + QString::number(answer_id);

    QJsonObject testJson = m_test->toJsonObject();
    testJson.insert("session", m_session->getJsonObject());

    for (int i = 0; i < m_test->getMeasurementCount(); i++) {
        const Measurement& measure = m_test->get(i);
        const QString &side = measure.getAttribute("EYE_SIDE_VENDOR").value().toString().trimmed();

        QString path = measure.getAttribute("EYE_PICT_VENDOR").value().toString().trimmed();
        path.replace("\\", "//");

        const QString &fileSize = FileUtils::getHumanReadableFileSize(path);

        qDebug() << "side" << side << " path: " << path << " size: " << fileSize;
        qDebug() << measure.toJsonObject();

        const QString fileName = "EYE_" + side + "_jpg";
        testJson.insert("files", QJsonObject {{ fileName, fileSize }});

        bool ok = NetworkUtils::sendHTTPSRequest("PATCH",
                                   (answerUrl + "?filename=EYE_"
                                                  + side + ".jpg").toStdString(),
                                   "application/octet-stream",
                                   FileUtils::readFile(
                                   measure.getAttribute("EYE_PICT_VENDOR").toString()));
        if (!ok)
            qCritical() << "RetinalCameraManager::finish: failed to send eye";
    }

    QJsonObject values;
    values.insert("value", testJson);

    const QJsonDocument jsonDoc(values);
    const QByteArray serializedData = jsonDoc.toJson();

    bool ok = NetworkUtils::sendHTTPSRequest("PATCH", answerUrl.toStdString(), "application/json", serializedData);

    if (ok) {
        emit success("Save successful. You may close this window.");
    }
    else {
        emit error("Could not save results, please contact support.");
    }

    m_db.close();
}

bool RetinalCameraManager::cleanupDatabase()
{
    qInfo() << "RetinalCameraManager::cleanupDatabase";

    if (!m_db.open()) {
        qCritical() << "RetinalCameraManager::cleanupDatabase: could not open database";
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare("SELECT FileName, FileExt, StoragePathUid FROM Media WHERE PatientUid = :patientUUID");
    query.bindValue(":patientUUID", defaultPatientUUID);

    qDebug() << query.lastQuery();

    if (!query.exec()) {
        qCritical() << "RetinalCameraManager::cleanupDatabase:" << query.lastError().text();
        return false;
    }

    while (query.next())
    {
        const QString storagePathUid = query.value("StoragePathUid").toString();
        const QString fileName = query.value("FileName").toString().trimmed();
        const QString extension = query.value("FileExt").toString().trimmed();
        const QString location = EyeExtractorQueryUtil::getLocation(m_db, storagePathUid);

        qDebug() << "RetinalCameraManager::cleanupDatabase - deleting:"
             << location + "/" + fileName + extension;

        QFile file(location + "/" + fileName + extension);
        file.remove();
    }

    query.prepare("DELETE FROM Exams WHERE PatientUid = :patientUUID");
    query.bindValue(":patientUUID", defaultPatientUUID);
    qDebug() << query.lastQuery();

    if (!query.exec()) {
        qCritical() << query.lastError().text();
        return false;
    }


    query.prepare("DELETE FROM Media WHERE PatientUid = :patientUUID");
    query.bindValue(":patientUUID", defaultPatientUUID);
    qDebug() << query.lastQuery();

    if (!query.exec()) {
        qCritical() << query.lastError().text();
        return false;
    }

    query.prepare("DELETE FROM Patients WHERE PatientUid = :patientUUID");
    query.bindValue(":patientUUID", defaultPatientUUID);
    qDebug() << query.lastQuery();

    if (!query.exec()) {
        qCritical() << "RetinalCameraManager::cleanupDatabase:" << query.lastError().text();
        return false;
    }

    query.prepare("DELETE FROM Persons WHERE PersonUid = :personUUID");
    query.bindValue(":personUUID", defaultPatientUUID);
    qDebug() << query.lastQuery();

    if (!query.exec()) {
        qCritical() << "RetinalCameraManager::cleanupDatabase:" << query.lastError().text();
        return false;
    }

    m_db.close();

    return true;
}

bool RetinalCameraManager::initializeDatabase()
{
    qInfo() << "RetinalCameraManager::initializeDatabase";

    m_db.open();

    QString participantId = m_session->getBarcode();
    QSqlQuery query(m_db);

    query.prepare("INSERT INTO Persons (PersonUid, SurName, ForeName) VALUES (:personUUID, "
                  ":lastName, :firstName)");
    query.bindValue(":personUUID", defaultPersonUUID);
    query.bindValue(":firstName", "CLSA");
    query.bindValue(":lastName", "Participant");
    qDebug() << query.lastQuery();
    if (!query.exec()) {
        qCritical() << "RetinalCameraManager::initializeDatabase:" << query.lastError().text();
        return false;
    }

    query.prepare("INSERT INTO Patients (PatientUid, PatientIdentifier, PersonUid) VALUES (:patientUUID, :participantId, :personUUID)");
    query.bindValue(":patientUUID", defaultPatientUUID);
    query.bindValue(":participantId", participantId);
    query.bindValue(":personUUID", defaultPatientUUID);
    qDebug() << query.lastQuery();

    if (!query.exec()) {
        qCritical() << "RetinalCameraManager::initializeDatabase:" << query.lastError().text();
        return false;
    }

    return true;
}

bool RetinalCameraManager::clearData()
{
    qInfo() << "RetinalCameraManager::clearData";

    m_test->reset();
    emit dataChanged(m_test);

    return false;
}

void RetinalCameraManager::readOutput()
{
    qInfo() << "RetinalCameraManager::readOutput";

    if (QProcess::NormalExit != m_process.exitStatus()) {
        qCritical() << "Process failed to finish correctly, cannot read output";
        emit error("Process failed to finish correctly, cannot read output");
        return;
    }

    auto session = qSharedPointerCast<RetinalCameraSession>(m_session);
    auto test = qSharedPointerCast<RetinalCameraTest>(m_test);

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
    qInfo() << "RetinalCameraManager::getLeftEye";

    QJsonObject results;
    QSqlQuery query(m_db);

    query.prepare(
        "SELECT FileName, FileExt, StoragePathUid, CreateDate FROM Media WHERE PatientUid = "
        ":patientUid AND EyeType = 1 AND Status = 1 AND Display = 1 ORDER BY CreateDate ASC");
    query.bindValue(":patientUid", defaultPatientUUID);
    qDebug() << query.lastQuery();

    if (!query.exec()) {
        qCritical() << "RetinalCameraManager::getLeftEye:" << query.lastError().text();
        emit error("Could not find a left eye image");
        return results;
    }

    if (!query.size()) {
        emit error("Could not find a left eye image");
        return results;
    }

    while (query.next()) {
        results["fileName"] = query.value(0).toString().trimmed();
        results["fileExt"] = query.value(1).toString().trimmed();
        results["side"] = "left";
        results["storagePathUid"] = query.value(2).toString().trimmed();
        results["createDate"] = query.value(3).toString().trimmed();

        qDebug() << results;
    }

    query.prepare("SELECT Location FROM StoragePaths WHERE StoragePathUid = :storagePathUid");
    query.bindValue(":storagePathUid", results["storagePathUid"].toString());

    qDebug() << query.lastQuery();

    if (!query.exec()) {
        qCritical() << "RetinalCameraManager::getLeftEye:" << query.lastError().text();
        emit error("Could not find a left eye image");
        return results;
    }

    while (query.next()) {
        results["filePath"] = query.value(0).toString().trimmed();
        qDebug() << results;
    }

    return results;
}

QJsonObject RetinalCameraManager::getRightEye()
{
    qInfo() << "RetinalCameraManager::getRightEye";

    QSqlQuery query(m_db);
    query.prepare(
        "SELECT FileName, FileExt, StoragePathUid, CreateDate FROM Media WHERE PatientUid = "
        ":patientUid AND EyeType = 2 AND Status = 1 AND Display = 1 ORDER BY CreateDate ASC");
    query.bindValue(":patientUid", defaultPatientUUID);

    qDebug() << query.lastQuery();
    if (!query.exec()) {
        qCritical() << "RetinalCameraManager::getRightEye:" << query.lastError().text();
    }

    QJsonObject results;
    if (!query.size()) {
        emit error("Could not find a right eye image");
        return results;
    }

    while (query.next()) {
        results["fileName"] = query.value(0).toString().trimmed();
        results["fileExt"] = query.value(1).toString().trimmed();
        results["side"] = "right";
        results["storagePathUid"] = query.value(2).toString().trimmed();
        results["createDate"] = query.value(3).toString().trimmed();

        qDebug() << "fileName" << query.value(0);
        qDebug() << "fileExt" << query.value(1);
        qDebug() << "storagePathUid" << query.value(2);
        qDebug() << "createDate" << query.value(3);
    }

    query.prepare("SELECT Location FROM StoragePaths WHERE StoragePathUid = :storagePathUid");
    query.bindValue(":storagePathUid", results["storagePathUid"].toString());
    qDebug() <<  query.lastQuery();

    if (!query.exec()) {
        qCritical() << "RetinalCameraManager::getRightEye:" << query.lastError().text();
        return results;
    }

    while (query.next()) {
        results["filePath"] = query.value(0).toString().trimmed();
        qDebug() << results;
    }

    return results;
}
