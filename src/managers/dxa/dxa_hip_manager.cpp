#include "cypress_session.h"

#include "auxiliary/file_utils.h"
#include "auxiliary/windows_util.h"
#include "auxiliary/network_utils.h"

#include "data/dxa/tests/dxa_hip_test.h"
#include "managers/dxa/dxa_hip_manager.h"

#include <QException>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMap>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QString>
#include <QVariant>



DxaHipManager::DxaHipManager(QSharedPointer<DxaHipSession> session)
    : ManagerBase(session)
{
    m_test.reset(new DxaHipTest);

    m_runnableName = CypressSettings::readSetting("dxa/dicom/runnableName").toString();
    m_runnablePath = CypressSettings::readSetting("dxa/dicom/runnablePath").toString();

    m_aeTitle = CypressSettings::readSetting("dxa/dicom/aeTitle").toString();
    m_host = CypressSettings::readSetting("dxa/dicom/host").toString();
    m_port = CypressSettings::readSetting("dxa/dicom/port").toString();

    m_storageDirPath = CypressSettings::readSetting("dxa/dicom/storagePath").toString();
    m_logConfigPath = CypressSettings::readSetting("dxa/dicom/log_config").toString();
    m_ascConfigPath = CypressSettings::readSetting("dxa/dicom/asc_config").toString();

    m_patscanDbPath = CypressSettings::readSetting("dxa/patscanDbPath").toString();
    m_refscanDbPath = CypressSettings::readSetting("dxa/refscanDbPath").toString();

    WindowsUtil::killProcessByName(L"storescp.exe");

    if (m_debug) {
        qDebug() << "DXAHipManager";

        qDebug() << session->getSessionId();
        qDebug() << session->getBarcode();
        qDebug() << session->getInterviewer();

        qDebug() << session->getInputData();
    }
}

DxaHipManager::~DxaHipManager()
{
    m_dicomServer->stop();
}

bool DxaHipManager::isInstalled()
{
    bool isDebugMode = CypressSettings::isDebugMode();
    bool isSimMode = CypressSettings::isSimMode();

    if (isSimMode)
        return true;

    QString runnableName = CypressSettings::readSetting("dxa/dicom/runnableName").toString();
    QString runnablePath = CypressSettings::readSetting("dxa/dicom/runnablePath").toString();
    QString aeTitle = CypressSettings::readSetting("dxa/dicom/aeTitle").toString();
    QString host = CypressSettings::readSetting("dxa/dicom/host").toString();
    QString port = CypressSettings::readSetting("dxa/dicom/port").toString();

    QString storageDirPath = CypressSettings::readSetting("dxa/dicom/storagePath").toString();
    QString logConfigPath = CypressSettings::readSetting("dxa/dicom/log_config").toString();
    QString ascConfigPath = CypressSettings::readSetting("dxa/dicom/asc_config").toString();

    QString patscanDbPath = CypressSettings::readSetting("dxa/patscanDbPath").toString();
    QString refscanDbPath = CypressSettings::readSetting("dxa/refscanDbPath").toString();

    if (runnableName.isNull() || runnableName.isEmpty()) {
        if (isDebugMode)
            qDebug() << "DxaHipManager::isInstalled - runnableName is not defined";
        return false;
    }

    if (runnablePath.isNull() || runnablePath.isEmpty()) {
        if (isDebugMode)
            qDebug() << "DxaHipManager::isInstalled - runnablePath is not defined";
        return false;
    }

    if (aeTitle.isNull() || aeTitle.isEmpty()) {
        if (isDebugMode)
            qDebug() << "DxaHipManager::isInstalled - aeTitle is not defined";
        return false;
    }

    if (host.isNull() || host.isEmpty()) {
        if (isDebugMode)
            qDebug() << "DxaHipManager::isInstalled - host is not defined";
        return false;
    }

    if (port.isNull() || port.isEmpty()) {
        if (isDebugMode)
            qDebug() << "DxaHipManager::isInstalled - port is not defined";
        return false;
    }

    if (storageDirPath.isNull() || storageDirPath.isEmpty()) {
        if (isDebugMode)
            qDebug() << "DxaHipManager::isInstalled - storageDirPath is not defined";
        return false;
    }

    if (logConfigPath.isNull() || logConfigPath.isNull()) {
        if (isDebugMode)
            qDebug() << "DxaHipManager::isInstalled - logConfigPath is not defined";
        return false;
    }

    if (ascConfigPath.isNull() || ascConfigPath.isEmpty()) {
        if (isDebugMode)
            qDebug() << "DxaHipManager::isInstalled - ascConfigPath is not defined";
        return false;
    }

    if (patscanDbPath.isNull() || patscanDbPath.isEmpty()) {
        if (isDebugMode)
            qDebug() << "DxaHipManager::isInstalled - patscanDbPath is not defined";

        return false;
    }

    QFileInfo patscanFile(patscanDbPath);
    if (!patscanFile.exists()) {
        if (isDebugMode)
            qDebug() << "DxaHipManager::isInstalled - patscan file does not exist: "
                     << patscanDbPath;
        return false;
    }

    if (!patscanFile.isFile()) {
        if (isDebugMode)
            qDebug() << "DxaHipManager::isInstalled - patscan file is not a file at"
                     << patscanDbPath;
        return false;
    }

    if (!patscanFile.isReadable()) {
        if (isDebugMode)
            qDebug() << "DxaHipManager::isInstalled - patscan file is not readable at "
                     << patscanDbPath;
        return false;
    }

    if (refscanDbPath.isNull() || refscanDbPath.isEmpty()) {
        if (isDebugMode)
            qDebug() << "DxaHipManager::isInstalled - refscanDbPath is not defined at "
                     << refscanDbPath;
        return false;
    }

    //QFileInfo refscanFile(refscanDbPath);
    //if (!refscanFile.exists()) {
    //    if (isDebugMode)
    //        qDebug() << "DxaHipManager::isInstalled - refscanDbPath is not defined at "
    //                 << refscanDbPath;
    //    return false;
    //}

    //if (!refscanFile.isFile()) {
    //    if (isDebugMode)
    //        qDebug() << "DxaHipManager::isInstalled - refscanDbPath is not a file at "
    //                 << refscanDbPath;
    //    return false;
    //}

    //if (!refscanFile.isReadable()) {
    //    if (isDebugMode)
    //        qDebug() << "DxaHipManager::isInstalled - refscanDbPath is not readable at "
    //                 << refscanDbPath;
    //    return false;
    //}

    QFileInfo exeInfo(runnableName);
    if (!exeInfo.exists()) {
        if (isDebugMode)
            qDebug() << "DxaHipManager::isInstalled - runnableName does not exist at"
                     << runnableName;
        return false;
    }
    if (!exeInfo.isExecutable()) {
        if (isDebugMode)
            qDebug() << "DxaHipManager::isInstalled - runnableName is not executable at"
                     << runnableName;
        return false;
    }

    QFileInfo workingDir(runnablePath);
    if (!workingDir.exists()) {
        if (isDebugMode)
            qDebug() << "DxaHipManager::isInstalled - working directory does not exist at"
                     << workingDir;
        return false;
    }

    if (!workingDir.isDir()) {
        if (isDebugMode)
            qDebug() << "DxaHipManager::isInstalled - working directory is not writable at"
                     << workingDir;
        return false;
    }

    if (!workingDir.isWritable()) {
        if (isDebugMode)
            qDebug() << "DxaHipManager::isInstalled - working directory is not writable at"
                     << workingDir;
        return false;
    }

    return true;
}

// what the manager does in response to the main application
// window invoking its run method
//
bool DxaHipManager::start()
{
    setUp();

    m_dicomServer->start();

    emit started(m_test);
    emit dataChanged(m_test);
    emit canMeasure();

    return true;
}

// Set up device
bool DxaHipManager::setUp()
{
    m_dicomServer.reset(
        new DcmRecv(m_runnableName, m_ascConfigPath, m_storageDirPath, m_aeTitle, m_port));

    connect(m_dicomServer.get(), &DcmRecv::running, this, [=]() {
        if (m_debug)
            qDebug() << "DxaHipManager::running - DICOM server is running...";
    });

    connect(m_dicomServer.get(),
            &DcmRecv::dicomFilesReceived,
            this,
            &DxaHipManager::dicomFilesReceived);

    return true;
}

void DxaHipManager::dicomFilesReceived(QList<DicomFile> dicomFiles)
{
    // pass received dicom files to test class
    QSharedPointer<DxaHipTest> test = qSharedPointerCast<DxaHipTest>(m_test);
    QSharedPointer<DxaHipSession> session = qSharedPointerCast<DxaHipSession>(m_session);

    test->fromDicomFiles(dicomFiles, *session);
    emit dataChanged(m_test);
}

// retrieve a measurement from the device
//
void DxaHipManager::measure()
{
    if (m_sim) {
        m_test->reset();
        m_test->simulate();

        emit dataChanged(m_test);
        emit canFinish();

        return;
    }

    QSharedPointer<DxaHipTest> test = qSharedPointerCast<DxaHipTest>(m_test);
    if (!test->hasAllNeededFiles()) {
        QMessageBox::warning(nullptr, "Warning", "Have not received all images from Hologic Apex");
        return;
    }

    QFileInfo patscanFileInfo(m_patscanDbPath);
    QString localPath = QDir::currentPath() + "/" + patscanFileInfo.fileName();
    qDebug() << m_patscanDbPath << localPath;

    if (patscanFileInfo.exists() && patscanFileInfo.isReadable()) {
        if (QFileInfo(localPath).exists()) {
            QFile::remove(localPath);
        }

        if (!QFile::copy(m_patscanDbPath, localPath)) {
            if (m_debug)
                qDebug() << "error copying patscan db from " << m_patscanDbPath << "to"
                         << localPath;
            return;
        }
    } else {
        qDebug() << "could not access patscanDb at" << m_patscanDbPath;
        emit error("Could not access PatScanDB on Apex workstation");
        return;
    }

    //// get refscan db variables for measurements
    //QFileInfo refscanFileInfo(m_refscanDbPath);
    //if (refscanFileInfo.exists() && refscanFileInfo.isReadable()) {
    //    QString localPath = QDir::currentPath() + "/" + refscanFileInfo.fileName();
    //    if (QFileInfo(localPath).exists()) {
    //        QFile::remove(localPath);
    //    }

    //    if (!QFile::copy(m_refscanDbPath, localPath)) {
    //        if (m_debug)
    //            qDebug() << "error copying refscan db from " << m_refscanDbPath << "to"
    //                     << localPath;
    //        return;
    //    }
    //} else {
    //    emit error("Could not access ReferenceDB on Apex workstation");
    //    return;
    //}

    // calculate totals and averages
    m_patscanDb = QSqlDatabase::addDatabase("QODBC");
    m_patscanDb.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS "
                                "Access};DBQ="
                                + localPath.replace("/", "\\"));

    if (!m_patscanDb.open()) {
        qDebug() << "Error: " << m_patscanDb.lastError().text();
        return;
        // Handle error
    } else {
        // Connection is successful, you can query the database
        qDebug() << "connected to the patscanDb at " << localPath;

        if (!m_patscanDb.open()) {
            qDebug() << "could not open patscanDB";
            return;
        }

        test->retrieveResults(m_patscanDb, m_session->getBarcode());

        m_patscanDb.close();
    }

    emit canFinish();
}

// implementation of final clean up of device after disconnecting and all
// data has been retrieved and processed by any upstream classes
//
void DxaHipManager::finish()
{
    QSharedPointer<DxaHipTest> test = qSharedPointerCast<DxaHipTest>(m_test);

    int answer_id = m_session->getAnswerId();
    QString barcode = m_session->getBarcode();

    // Hip
    QString hip_1_file_name = "L_HIP_DICOM.dcm";
    QString hip_2_file_name = "R_HIP_DICOM.dcm";

    QJsonObject testJson = m_test->toJsonObject();
    QJsonObject sessionObj = m_session->getJsonObject();
    QJsonObject metadata = m_test->getMetaData().toJsonObject();

    QJsonObject files = {};

    if (!testJson["l_hip"].toObject().isEmpty()) {
        files.insert(hip_1_file_name.replace(QRegExp(".dcm"), ""),
                     FileUtils::getHumanReadableFileSize(
                         test->leftHipMeasurement->m_hipDicomFile.absFilePath));
    }

    if (!testJson["r_hip"].toObject().isEmpty()) {
        files.insert(hip_2_file_name.replace(QRegExp(".dcm"), ""),
                     FileUtils::getHumanReadableFileSize(
                         test->rightHipMeasurement->m_hipDicomFile.absFilePath));
    }

    testJson.insert("session", sessionObj);
    testJson.insert("files", files);

    QJsonObject responseJson = {};
    responseJson.insert("value", testJson);

    QJsonDocument jsonDoc(responseJson);
    QByteArray serializedData = jsonDoc.toJson();

    QString host = CypressSettings::getPineHost();
    QString endpoint = CypressSettings::getPineEndpoint();

    bool ok = false;
    ok = NetworkUtils::sendHTTPSRequest(
        Poco::Net::HTTPRequest::HTTP_PATCH,
        host.toStdString() + endpoint.toStdString() + QString::number(answer_id).toStdString(),
        "application/json",
        serializedData
    );

    if (!ok) {
        emit error("Something went wrong, please contact support.");
        return;
    }

    if (!testJson["l_hip"].toObject().isEmpty()) {

        QByteArray leftHipDicomFile = FileUtils::readFile(
            test->leftHipMeasurement->m_hipDicomFile.absFilePath);

        ok = NetworkUtils::sendHTTPSRequest(
            Poco::Net::HTTPRequest::HTTP_PATCH,
            host.toStdString() + endpoint.toStdString() + QString::number(answer_id).toStdString() + "?filename=" + hip_1_file_name.toStdString() + ".dcm",
            "application/octet-stream",
            leftHipDicomFile
        );

        if (!ok) {
            qDebug() << "PATCH dicom failed";
            return;
        }
    }

    if (!testJson["r_hip"].toObject().isEmpty()) {
        QByteArray rightHipDicomFile = FileUtils::readFile(
            test->rightHipMeasurement->m_hipDicomFile.absFilePath);

        ok = NetworkUtils::sendHTTPSRequest(
            Poco::Net::HTTPRequest::HTTP_PATCH,
            host.toStdString() + endpoint.toStdString() + QString::number(answer_id).toStdString()
            + "?filename=" + hip_1_file_name.toStdString() + ".dcm",
            "application/octet-stream",
            rightHipDicomFile
        );

        if (!ok) {
            qDebug() << "PATCH dicom failed";
            return;
        }
    }

    emit success("Saved images to Pine, you may now close this window");

    cleanUp();
}

bool DxaHipManager::cleanUp()
{
    return clearData();
}

bool DxaHipManager::clearData()
{
    m_test->reset();
    return true;
}
