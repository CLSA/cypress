#include "cypress_session.h"

#include "data/dxa/tests/dxa_hip_test.h"
#include "managers/dxa/dxa_hip_manager.h"

#include "auxiliary/file_utils.h"
#include "auxiliary/windows_util.h"
#include "auxiliary/network_utils.h"

#include <QApplication>
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

    qInfo() << "DXAHipManager";
    qInfo() << session->getSessionId();
    qInfo() << session->getBarcode();
    qInfo() << session->getInterviewer();
    qInfo() << session->getInputData();
}

DxaHipManager::~DxaHipManager()
{
    m_dicomServer->stop();
}

bool DxaHipManager::isInstalled() {
    qDebug() << "DxaHipManager::isInstalled";
    if (CypressSettings::isSimMode())
        return true;

    const QString runnableName   = CypressSettings::readSetting("dxa/dicom/runnableName").toString();
    const QString runnablePath   = CypressSettings::readSetting("dxa/dicom/runnablePath").toString();
    const QString aeTitle        = CypressSettings::readSetting("dxa/dicom/aeTitle").toString();
    const QString host           = CypressSettings::readSetting("dxa/dicom/host").toString();
    const QString port           = CypressSettings::readSetting("dxa/dicom/port").toString();

    const QString storageDirPath = CypressSettings::readSetting("dxa/dicom/storagePath").toString();
    const QString logConfigPath  = CypressSettings::readSetting("dxa/dicom/log_config").toString();
    const QString ascConfigPath  = CypressSettings::readSetting("dxa/dicom/asc_config").toString();

    const QString patscanDbPath  = CypressSettings::readSetting("dxa/patscanDbPath").toString();
    const QString refscanDbPath  = CypressSettings::readSetting("dxa/refscanDbPath").toString();

    if (runnableName.isNull() || runnableName.isEmpty()) {
        qDebug() << "runnableName is not defined";
        return false;
    }

    if (runnablePath.isNull() || runnablePath.isEmpty()) {
        qDebug() << "runnablePath is not defined";
        return false;
    }

    if (aeTitle.isNull() || aeTitle.isEmpty()) {
        qDebug() << "aeTitle is not defined";
        return false;
    }

    if (host.isNull() || host.isEmpty()) {
        qDebug() << "host is not defined";
        return false;
    }

    if (port.isNull() || port.isEmpty()) {
        qDebug() << "port is not defined";
        return false;
    }

    if (storageDirPath.isNull() || storageDirPath.isEmpty()) {
        qDebug() << "storageDirPath is not defined";
        return false;
    }

    if (logConfigPath.isNull() || logConfigPath.isNull()) {
        qDebug() << "logConfigPath is not defined";
        return false;
    }

    if (ascConfigPath.isNull() || ascConfigPath.isEmpty()) {
        qDebug() << "ascConfigPath is not defined";
        return false;
    }

    if (patscanDbPath.isNull() || patscanDbPath.isEmpty()) {
        qDebug() << "patscanDbPath is not defined";
        return false;
    }

    const QFileInfo patscanFile(patscanDbPath);
    if (!patscanFile.exists()) {
        qDebug() << "patscan file does not exist at" << patscanDbPath;
        return false;
    }

    if (!patscanFile.isFile()) {
        qDebug() << "patscan file is not a file at" << patscanDbPath;
        return false;
    }

    if (!patscanFile.isReadable()) {
        qDebug() << "patscan file is not readable at" << patscanDbPath;
        return false;
    }

    if (refscanDbPath.isNull() || refscanDbPath.isEmpty()) {
        qDebug() << "refscanDbPath is not defined at" << refscanDbPath;
        return false;
    }

    const QFileInfo refscanFile(refscanDbPath);
    if (!refscanFile.exists()) {
        qDebug() << "refscanDbPath is not defined at" << refscanDbPath;
        return false;
    }

    if (!refscanFile.isFile()) {
        qDebug() << "refscanDbPath is not a file at" << refscanDbPath;
        return false;
    }

    if (!refscanFile.isReadable()) {
        qDebug() << "refscanDbPath is not readable at" << refscanDbPath;
        return false;
    }

    const QFileInfo exeInfo(runnableName);
    if (!exeInfo.exists()) {
        qDebug() << "runnableName does not exist at" << runnableName;
        return false;
    }
    if (!exeInfo.isExecutable()) {
        qDebug() << "runnableName is not executable at" << runnableName;
        return false;
    }

    const QFileInfo workingDir(runnablePath);
    if (!workingDir.exists()) {
        qDebug() << "working directory does not exist at" << workingDir;
        return false;
    }

    if (!workingDir.isDir()) {
        qDebug() << "working directory is not writable at" << workingDir;
        return false;
    }

    if (!workingDir.isWritable()) {
        qDebug() << "working directory is not writable at" << workingDir;
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

    //emit status("Ready");

    emit started(m_test);
    emit dataChanged(m_test);

    return true;
}

// Set up device
bool DxaHipManager::setUp()
{
    m_dicomServer.reset(
        new DcmRecv(m_runnableName, m_ascConfigPath, m_storageDirPath, m_aeTitle, m_port));

    connect(m_dicomServer.get(), &DcmRecv::running, this, [=]() {
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
    //emit status(QString(QString("Received %1 files").arg(dicomFiles.length())));

    // pass received dicom files to test class
    auto test = qSharedPointerCast<DxaHipTest>(m_test);
    auto session = qSharedPointerCast<DxaHipSession>(m_session);

    int filesReceived = test->fromDicomFiles(dicomFiles, *session);

    emit status(QString("Received %1 files").arg(filesReceived));

    emit dataChanged(m_test);

    emit canMeasure();
}

// retrieve a measurement from the device
//
void DxaHipManager::measure()
{
    //emit transfer();
    if (m_sim) {
        m_test->reset();
        m_test->simulate();

        emit dataChanged(m_test);
        emit canFinish();

        return;
    }

    auto test = qSharedPointerCast<DxaHipTest>(m_test);
    if (!test->hasAllNeededFiles()) {
        QMessageBox::warning(nullptr, "Warning", "Have not received all images from Hologic Apex");
    }

    emit status("Copying files from DEXA (1/2)");
    QApplication::processEvents();

    // Copy the patient scan mdb file from Apex
    if (!initPatScanDb()) {
        emit error("Failed to copy the PatScan.mdb file from the Apex workstation");
        return;
    }

    emit status("Copying files from DEXA (2/2)");
    QApplication::processEvents();
    // Copy the reference mdb file from Apex
    if (!initReferenceDb()) {
        emit error("Failed to copy the reference.mdb file from the Apex workstation");
        return;
    }

    emit status("Gathering variables");
    QApplication::processEvents();

    test->getPatientScan(m_patscanDb, m_session->getBarcode());

    QString patientKey = test->getMetaData("PATIENT_KEY").toString();
    QJsonObject patientData {
        { "PATIENT_KEY", patientKey },
        { "BIRTHDATE", test->getMetaData("BIRTHDATE").toString() },
        { "SEX", test->getMetaData("SEX").toString() },
        { "ETHNICITY", test->getMetaData("ETHNICITY").toString() }
    };

    test->leftHipMeasurement->getScanAnalysisData(m_patscanDb, m_referenceDb, patientData);
    test->rightHipMeasurement->getScanAnalysisData(m_patscanDb, m_referenceDb, patientData);

    m_patscanDb.close();
    m_referenceDb.close();

    emit canFinish();
    emit status("Ready to submit");
}

// implementation of final clean up of device after disconnecting and all
// data has been retrieved and processed by any upstream classes
//
void DxaHipManager::finish() {
    bool ok = false;

    auto test = qSharedPointerCast<DxaHipTest>(m_test);
    const int answer_id = m_session->getAnswerId();
    const QString pineOrigin = m_session->getOrigin();
    const QString answerUrl = pineOrigin + "/answer/" + QString::number(answer_id);

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
        const QByteArray leftHipDicomFile = FileUtils::readFile(
            test->leftHipMeasurement->m_hipDicomFile.absFilePath);

        ok = NetworkUtils::sendHTTPSRequest(
            Poco::Net::HTTPRequest::HTTP_PATCH,
            answerUrl.toStdString() + "?filename=" + hip_1_file_name.replace("_dcm", ".dcm").toStdString(),
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
        const QByteArray rightHipDicomFile = FileUtils::readFile(
            test->rightHipMeasurement->m_hipDicomFile.absFilePath);

        ok = NetworkUtils::sendHTTPSRequest(
            Poco::Net::HTTPRequest::HTTP_PATCH,
            answerUrl.toStdString()
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

    const QJsonDocument jsonDoc(responseJson);
    const QByteArray serializedData = jsonDoc.toJson();

    ok = NetworkUtils::sendHTTPSRequest(
        Poco::Net::HTTPRequest::HTTP_PATCH,
        answerUrl.toStdString(),
        "application/json",
        serializedData
    );

    qDebug() << responseJson;

    if (!ok)
        emit error("Could not send results to Pine");
    else
        emit success("Saved images to Pine, you may now close this window");

    cleanUp();
}

bool DxaHipManager::cleanUp() {
    return clearData();
}


bool DxaHipManager::initPatScanDb() {
    if (!copyPatScanDb())
        return false;

    m_patscanDb = QSqlDatabase::addDatabase("QODBC", "patscan");
    m_patscanDb.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};DBQ="
                              + QDir::toNativeSeparators(m_patscanDbFileInfo.absoluteFilePath()));
    if (!m_patscanDb.open())
        return false;

    return true;
}

bool DxaHipManager::initReferenceDb() {
    if (!copyReferenceDb())
        return false;

    m_referenceDb = QSqlDatabase::addDatabase("QODBC", "reference");
    m_referenceDb.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};DBQ="
                                + QDir::toNativeSeparators(m_referenceDbFileInfo.absoluteFilePath()));
    if (!m_referenceDb.open())
        return false;

    return true;
}

bool DxaHipManager::copyPatScanDb() {
    // Copies the PatScan.mdb file from the DEXA/APEX workstation and sets up a reference to the file.
    // Assumes that the Apex workstation is sharing the file

    // Returns true if successful, otherwise false
    const QFileInfo patscanFileInfo(m_patscanDbPath);
    if (!patscanFileInfo.exists())
        return false;

    if (!patscanFileInfo.isReadable())
        return false;

    const QFileInfo localPatScanFileInfo(QDir::current().absoluteFilePath(patscanFileInfo.fileName()));
    if (localPatScanFileInfo.exists()) {
        if (!QFile::remove(localPatScanFileInfo.absoluteFilePath())) {
            return false;
        }
    }

    if (!QFile::copy(m_patscanDbPath, localPatScanFileInfo.absoluteFilePath())) {
        qDebug() << "error copying patscan db from " << m_refscanDbPath << "to" << localPatScanFileInfo.absoluteFilePath();
        return false;
    }

    m_patscanDbFileInfo = localPatScanFileInfo;

    return true;
}


bool DxaHipManager::copyReferenceDb() {
    // Copies the reference.mdb file from the DEXA computer, assumes that the Apex workstation is sharing the file
    //
    const QFileInfo apexReferenceFileInfo(m_refscanDbPath);
    if (!apexReferenceFileInfo.exists())
        return false;

    if (!apexReferenceFileInfo.isReadable())
        return false;

    const QFileInfo localReferenceFileInfo(QDir::current().absoluteFilePath(apexReferenceFileInfo.fileName()));
    if (localReferenceFileInfo.exists()) {
        if (!QFile::remove(localReferenceFileInfo.absoluteFilePath())) {
            qDebug() << "could not remove existing local reference db at: " << m_refscanDbPath;
            return false;
        }
    }

    if (!QFile::copy(m_refscanDbPath, localReferenceFileInfo.absoluteFilePath())) {
        qDebug() << "error copying refscan db from " << m_refscanDbPath << "to" << localReferenceFileInfo.absoluteFilePath();
        return false;
    }

    m_referenceDbFileInfo = localReferenceFileInfo;

    return true;
}

bool DxaHipManager::clearData() {
    m_test->reset();
    return true;
}
