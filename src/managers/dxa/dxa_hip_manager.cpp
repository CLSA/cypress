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
            qDebug() << "runnableName is not defined";
        return false;
    }

    if (runnablePath.isNull() || runnablePath.isEmpty()) {
        if (isDebugMode)
            qDebug() << "runnablePath is not defined";
        return false;
    }

    if (aeTitle.isNull() || aeTitle.isEmpty()) {
        if (isDebugMode)
            qDebug() << "aeTitle is not defined";
        return false;
    }

    if (host.isNull() || host.isEmpty()) {
        if (isDebugMode)
            qDebug() << "host is not defined";
        return false;
    }

    if (port.isNull() || port.isEmpty()) {
        if (isDebugMode)
            qDebug() << "port is not defined";
        return false;
    }

    if (storageDirPath.isNull() || storageDirPath.isEmpty()) {
        if (isDebugMode)
            qDebug() << "storageDirPath is not defined";
        return false;
    }

    if (logConfigPath.isNull() || logConfigPath.isNull()) {
        if (isDebugMode)
            qDebug() << "logConfigPath is not defined";
        return false;
    }

    if (ascConfigPath.isNull() || ascConfigPath.isEmpty()) {
        if (isDebugMode)
            qDebug() << "ascConfigPath is not defined";
        return false;
    }

    if (patscanDbPath.isNull() || patscanDbPath.isEmpty()) {
        if (isDebugMode)
            qDebug() << "patscanDbPath is not defined";

        return false;
    }

    QFileInfo patscanFile(patscanDbPath);
    if (!patscanFile.exists()) {
        if (isDebugMode)
            qDebug() << "patscan file does not exist: "
                     << patscanDbPath;
        return false;
    }

    if (!patscanFile.isFile()) {
        if (isDebugMode)
            qDebug() << "patscan file is not a file at"
                     << patscanDbPath;
        return false;
    }

    if (!patscanFile.isReadable()) {
        if (isDebugMode)
            qDebug() << "patscan file is not readable at "
                     << patscanDbPath;
        return false;
    }

    if (refscanDbPath.isNull() || refscanDbPath.isEmpty()) {
        if (isDebugMode)
            qDebug() << "refscanDbPath is not defined at "
                     << refscanDbPath;
        return false;
    }

    QFileInfo refscanFile(refscanDbPath);
    if (!refscanFile.exists()) {
        if (isDebugMode)
            qDebug() << "refscanDbPath is not defined at "
                     << refscanDbPath;
        return false;
    }

    if (!refscanFile.isFile()) {
        if (isDebugMode)
            qDebug() << "refscanDbPath is not a file at "
                     << refscanDbPath;
        return false;
    }

    if (!refscanFile.isReadable()) {
        if (isDebugMode)
            qDebug() << "refscanDbPath is not readable at "
                     << refscanDbPath;
        return false;
    }

    QFileInfo exeInfo(runnableName);
    if (!exeInfo.exists()) {
        if (isDebugMode)
            qDebug() << "runnableName does not exist at"
                     << runnableName;
        return false;
    }
    if (!exeInfo.isExecutable()) {
        if (isDebugMode)
            qDebug() << "runnableName is not executable at"
                     << runnableName;
        return false;
    }

    QFileInfo workingDir(runnablePath);
    if (!workingDir.exists()) {
        if (isDebugMode)
            qDebug() << "working directory does not exist at"
                     << workingDir;
        return false;
    }

    if (!workingDir.isDir()) {
        if (isDebugMode)
            qDebug() << "working directory is not writable at"
                     << workingDir;
        return false;
    }

    if (!workingDir.isWritable()) {
        if (isDebugMode)
            qDebug() << "working directory is not writable at"
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
    emit canMeasure();
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

    // get patscan db variables for measurements
    QFileInfo patscanFileInfo(m_patscanDbPath);
    QString localPatscanPath= QDir::currentPath() + "/" + patscanFileInfo.fileName();
    if (patscanFileInfo.exists() && patscanFileInfo.isReadable()) {
        if (QFileInfo(localPatscanPath).exists()) {
            QFile::remove(localPatscanPath);
        }
        if (m_debug)
            qDebug() << localPatscanPath;

        if (!QFile::copy(m_patscanDbPath, localPatscanPath)) {
            if (m_debug)
                qDebug() << "error copying patscan db from " << m_patscanDbPath << "to"
                         << localPatscanPath;
        }
    } else {
        qDebug() << "could not access patscanDb at" << m_patscanDbPath;
        emit error("Could not access Apex workstation");
        return;
    }

    // get refscan db variables for measurements
    QFileInfo refscanFileInfo(m_refscanDbPath);
    QString localRefScanPath = QDir::currentPath() + "/" + refscanFileInfo.fileName();

    if (refscanFileInfo.exists() && refscanFileInfo.isReadable()) {
        if (QFileInfo(localRefScanPath).exists()) {
            QFile::remove(localRefScanPath);
        }
        if (m_debug)
            qDebug() << localRefScanPath;

        if (!QFile::copy(m_refscanDbPath, localRefScanPath)) {
            if (m_debug)
                qDebug() << "error copying refscan db from " << m_refscanDbPath << "to"
                         << localRefScanPath;
        }
    } else {
        emit error("Could not access Apex workstation");
        return;
    }

    // calculate totals and averages
    QSqlDatabase patscanDb = QSqlDatabase::addDatabase("QODBC", "patscan");
    patscanDb.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};DBQ="
                              + QDir::toNativeSeparators(localPatscanPath));
    if (!patscanDb.open()) {
        qDebug() << "Error: " << patscanDb.lastError().text();
        emit error("Could not open PatScan.mdb");
    } else {
        qDebug() << "connected to the patscanDb at " << localPatscanPath;
    }


    QSqlDatabase referenceDb = QSqlDatabase::addDatabase("QODBC", "reference");
    referenceDb.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};DBQ="
                                + QDir::toNativeSeparators(localRefScanPath));
    if (!referenceDb.open()) {
        qDebug() << "Error: " << patscanDb.lastError().text();
        emit error("Could not open reference.mdb");
    } else {
        qDebug() << "connected to the reference.mdb at " << localRefScanPath;
    }

    test->getPatientScan(patscanDb, m_session->getBarcode());

    QString patientKey = test->getMetaData("PATIENT_KEY").toString();
    QJsonObject patientData {
        { "PATIENT_KEY", patientKey },
        { "BIRTHDATE", test->getMetaData("BIRTHDATE").toString() },
        { "SEX", test->getMetaData("SEX").toString() },
        { "ETHNICITY", test->getMetaData("ETHNICITY").toString() }
    };

    test->leftHipMeasurement->getScanAnalysisData(patscanDb, referenceDb, patientData);
    test->rightHipMeasurement->getScanAnalysisData(patscanDb, referenceDb, patientData);

    emit canFinish();

    patscanDb.close();
    referenceDb.close();
}

// implementation of final clean up of device after disconnecting and all
// data has been retrieved and processed by any upstream classes
//
void DxaHipManager::finish()
{
    QSharedPointer<DxaHipTest> test = qSharedPointerCast<DxaHipTest>(m_test);
    bool ok = false;

    int answer_id = m_session->getAnswerId();
    QString barcode = m_session->getBarcode();

    QString host = CypressSettings::getPineHost();
    QString endpoint = CypressSettings::getPineEndpoint();

    // Hip
    QString hip_1_file_name = "L_HIP_DICOM.dcm";
    QString hip_2_file_name = "R_HIP_DICOM.dcm";

    QJsonObject testJson = m_test->toJsonObject();
    QJsonObject sessionObj = m_session->getJsonObject();
    QJsonObject metadata = m_test->getMetaData().toJsonObject();
    QJsonObject files = {};

    if (!testJson.value("results").toObject().value("hip_l").toObject().isEmpty()) {
        files.insert(hip_1_file_name.replace(".", "_"),
                     FileUtils::getHumanReadableFileSize(
                         test->leftHipMeasurement->m_hipDicomFile.absFilePath));
    }

    if (!testJson.value("results").toObject().value("hip_r").toObject().isEmpty()) {
        files.insert(hip_2_file_name.replace(".", "_"),
                     FileUtils::getHumanReadableFileSize(
                         test->rightHipMeasurement->m_hipDicomFile.absFilePath));
    }

    if (!testJson.value("results").toObject().value("hip_l").toObject().isEmpty()) {

        QByteArray leftHipDicomFile = FileUtils::readFile(
            test->leftHipMeasurement->m_hipDicomFile.absFilePath);

        ok = NetworkUtils::sendHTTPSRequest(
            Poco::Net::HTTPRequest::HTTP_PATCH,
            host.toStdString() + endpoint.toStdString() + QString::number(answer_id).toStdString() + "?filename=" + hip_1_file_name.replace("_dcm", ".dcm").toStdString(),
            "application/octet-stream",
            leftHipDicomFile
        );

        metadata.insert("femoral_neck_bmd", testJson.value("results").toObject().value("hip_l").toObject().value("neck_bmd").toDouble());

        if (!ok) {
            qDebug() << "PATCH dicom failed";
            return;
        }
    }

    if (!testJson.value("results").toObject().value("hip_r").toObject().isEmpty()) {
        QByteArray rightHipDicomFile = FileUtils::readFile(
            test->rightHipMeasurement->m_hipDicomFile.absFilePath);

        ok = NetworkUtils::sendHTTPSRequest(
            Poco::Net::HTTPRequest::HTTP_PATCH,
            host.toStdString() + endpoint.toStdString() + QString::number(answer_id).toStdString()
                + "?filename=" + hip_2_file_name.replace("_dcm", ".dcm").toStdString(),
            "application/octet-stream",
            rightHipDicomFile
        );

        metadata.insert("femoral_neck_bmd", testJson.value("results").toObject().value("hip_r").toObject().value("neck_bmd").toDouble());

        if (!ok) {
            qDebug() << "PATCH dicom failed";
            return;
        }
    }

    testJson.insert("session", sessionObj);
    testJson.insert("files", files);
    testJson.insert("metadata", metadata);

    QJsonObject responseJson = {};
    responseJson.insert("value", testJson);

    QJsonDocument jsonDoc(responseJson);
    QByteArray serializedData = jsonDoc.toJson();

    ok = NetworkUtils::sendHTTPSRequest(
        Poco::Net::HTTPRequest::HTTP_PATCH,
        host.toStdString() + endpoint.toStdString() + QString::number(answer_id).toStdString(),
        "application/json",
        serializedData
    );

    if (!ok) {
        emit error("Could not send results to Pine");
    }
    else {
        emit success("Saved images to Pine, you may now close this window");
    }

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
