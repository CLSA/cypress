#include "cypress_session.h"

#include "../../auxiliary/file_utils.h"

#include "data/dxa/tests/dxa_hip_test.h"
#include "managers/dxa/dxa_hip_manager.h"

#include <QException>
#include <QJsonDocument>
#include <QMap>
#include <QProcess>
#include <QSettings>
#include <QSql>
#include <QString>
#include <QVariant>

DxaHipManager::DxaHipManager(QSharedPointer<DxaHipSession> session) /* : m_dicomWatcher(QDir::currentPath())*/
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

    m_dicomServer.reset(
        new DcmRecv(m_runnableName, m_ascConfigPath, m_storageDirPath, m_aeTitle, m_port));

    connect(m_dicomServer.get(), &DcmRecv::running, this, [=]() {
        qDebug() << "DxaHipManager::running - DICOM server is running...";
    });

    connect(m_dicomServer.get(),
            &DcmRecv::dicomFilesReceived,
            this,
            &DxaHipManager::dicomFilesReceived);
}

DxaHipManager::~DxaHipManager()
{
    m_dicomServer->stop();
}

void DxaHipManager::databaseCopied(QFileInfo fileInfo)
{
    // Get name of files
    QString patscanDbFileName = "";
    QString refDbFileName = "";
}

bool DxaHipManager::isInstalled()
{
    return true;
}

// what the manager does in response to the main application
// window invoking its run method
//
void DxaHipManager::start()
{
    m_dicomServer->start();

    emit started(m_test.get());
    emit dataChanged(m_test.get());
    emit canMeasure();
}

// retrieve a measurement from the device
//
void DxaHipManager::measure()
{
    m_test->reset();

    if (CypressSettings::isSimMode()) {
        m_test->simulate();

        emit dataChanged(m_test.get());
        emit canFinish();

        return;
    }

    // copy over patscan and refscan db
    // m_networkFileCopier->copyFileFromSMB(QUrl(), "");
    // m_networkFileCopier->copyFileFromSMB(QUrl(), "");

    // pass dicom files to test class
    if (m_test->isValid()) {
        emit dataChanged(m_test.get());
        emit canFinish();
    }
}

// implementation of final clean up of device after disconnecting and all
// data has been retrieved and processed by any upstream classes
//
void DxaHipManager::finish()
{
    DxaHipTest *test = static_cast<DxaHipTest *>(m_test.get());

    int answer_id = m_session->getAnswerId();
    QString barcode = m_session->getBarcode();

    // Hip
    QString hip_1_file_name = "HIP_DICOM.dcm";

    QJsonObject testJson = m_test->toJsonObject();
    QJsonObject sessionObj = m_session->getJsonObject();
    QJsonObject metadata = m_test->getMetaData().toJsonObject();

    QJsonObject files = {};
    files.insert(hip_1_file_name.replace(QRegExp(".dcm"), ""),
                 FileUtils::getHumanReadableFileSize(
                     test->hipMeasurement->m_hipDicomFile.fileInfo.absoluteFilePath()));

    testJson.insert("session", sessionObj);
    testJson.insert("files", files);

    QJsonObject responseJson = {};
    responseJson.insert("value", testJson);

    QJsonDocument jsonDoc(responseJson);
    QByteArray serializedData = jsonDoc.toJson();

    QString host = CypressSettings::getPineHost();
    QString endpoint = CypressSettings::getPineEndpoint();

    sendHTTPSRequest("PATCH",
                     host + endpoint + QString::number(answer_id),
                     "application/json",
                     serializedData);

    QByteArray hipDicomFile = FileUtils::readFile(
        test->hipMeasurement->m_hipDicomFile.fileInfo.absoluteFilePath());
    sendHTTPSRequest("PATCH",
                     host + endpoint + QString::number(answer_id) + "?filename=" + hip_1_file_name + ".dcm",
                     "application/octet-stream",
                     hipDicomFile);

    emit success("");
}

void DxaHipManager::dicomFilesReceived()
{
    // pass received dicom files to test class
    DxaHipTest* test = static_cast<DxaHipTest*>(m_test.get());
    test->fromDicomFiles(m_dicomServer->receivedFiles);

    emit dataChanged(m_test.get());
}

QMap<QString, QVariant> DxaHipManager::retrieveDeviceData()
{
    return QVariantMap();
}


QMap<QString, QVariant> DxaHipManager::extractScanAnalysisData()
{
    return QVariantMap();
}


QMap<QString, QVariant> DxaHipManager::computeTandZScores()
{
    return QVariantMap();
}


QVariantMap DxaHipManager::getParticipantData()
{
    return QVariantMap();
}

// Set up device
bool DxaHipManager::setUp()
{
    return true;
}

bool DxaHipManager::cleanUp()
{
    return true;
}

bool DxaHipManager::clearData()
{
    return true;
}
