#include "cypress_session.h"

#include "dicom/dcm_recv.h"
#include "data/dxa/tests/dxa_test.h"
#include "managers/dxa/dxa_manager.h"

#include "../../auxiliary/file_utils.h"

#include <QException>
#include <QJsonDocument>
#include <QMap>
#include <QProcess>
#include <QSettings>
#include <QSql>
#include <QString>
#include <QVariant>

DXAManager::DXAManager(QSharedPointer<DXASession> session) /* : m_dicomWatcher(QDir::currentPath())*/
    : ManagerBase(session)
{
    m_test.reset(new DXATest);

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

    connect(m_dicomServer.get(),
            &DcmRecv::dicomFilesReceived,
            this,
            &DXAManager::dicomFilesReceived);
}

DXAManager::~DXAManager()
{
}

void DXAManager::copiedDatabaseFile(QFileInfo file)
{
    if (file.fileName() == "PatScan.mdb") {
        m_patscanDb = file;
    } else if (file.fileName() == "Reference.mdb") {
        m_referenceDb = file;
    } else {
        qDebug() << "file is not PatScan.mdb or Reference.mdb";
    }

    if (m_patscanDb.exists() && m_referenceDb.exists()) {
        emit canFinish();
    }
}

void DXAManager::dicomFilesReceived()
{
    const QList<DicomFile> &files = m_dicomServer->receivedFiles;

    // pass received dicom files to test class
    DXATest* test = static_cast<DXATest*>(m_test.get());
    test->fromDicomFiles(m_dicomServer->receivedFiles);

    if (test->areDicomFilesValid()) {
        emit canMeasure();
    }

    emit updateDicomTable(files);
}

bool DXAManager::isInstalled()
{
    return true;
}

// what the manager does in response to the main application
// window invoking its run method
//
void DXAManager::start()
{
    m_dicomServer->start();

    emit started(m_test.get());
    emit dataChanged(m_test.get());
    emit canMeasure();
}

// retrieve a measurement from the device
//
void DXAManager::measure()
{
    if (CypressSettings::isSimMode()) {
        m_test->simulate();

        emit dataChanged(m_test.get());
        emit canFinish();

        return;
    }

    //qDebug() << "measuring";
    // copy over patscan and refscandbs
    //m_networkFileCopier->copyFileFromSMB(QUrl(), "");
    //m_networkFileCopier->copyFileFromSMB(QUrl(), "");

    // check if valid
    DXATest *test = static_cast<DXATest *>(m_test.get());
    if (test->areDicomFilesValid()) {
        emit dataChanged(m_test.get());
        emit canMeasure();
    }
}

// implementation of final clean up of device after disconnecting and all
// data has been retrieved and processed by any upstream classes
//
void DXAManager::finish()
{
    int answer_id = m_session->getAnswerId();
    QString barcode = m_session->getBarcode();

    DXATest *test = static_cast<DXATest *>(m_test.get());

    // Whole body
    QByteArray wb_1 = FileUtils::readFile(
        test->wholeBodyMeasurement->m_wholeBody1.fileInfo.absoluteFilePath());
    QByteArray wb_2 = FileUtils::readFile(
        test->wholeBodyMeasurement->m_wholeBody2.fileInfo.absoluteFilePath());

    QString wb_1_file_name = test->wholeBodyMeasurement->m_wholeBody1.name + ".dcm";
    QString wb_2_file_name = test->wholeBodyMeasurement->m_wholeBody2.name + ".dcm";

    QByteArray sp_1 = FileUtils::readFile(
        test->apSpineMeasurement->m_apSpineFile.fileInfo.absoluteFilePath());

    // AP Lumbar Spine
    QString sp_1_file_name = test->apSpineMeasurement->m_apSpineFile.name + ".dcm";

    QByteArray iva_ot = FileUtils::readFile(
        test->ivaImagingMeasurement->m_dicomOtFile.fileInfo.absoluteFilePath());
    QByteArray iva_pr = FileUtils::readFile(
        test->ivaImagingMeasurement->m_dicomPrFile.fileInfo.absoluteFilePath());
    QByteArray iva_measure = FileUtils::readFile(
        test->ivaImagingMeasurement->m_dicomMeasureFile.fileInfo.absoluteFilePath());

    // IVA
    QString iva_ot_file_name = test->ivaImagingMeasurement->m_dicomOtFile.name + ".dcm";
    QString iva_pr_file_name = test->ivaImagingMeasurement->m_dicomPrFile.name + ".dcm";
    QString iva_measure_file_name = test->ivaImagingMeasurement->m_dicomMeasureFile.name + ".dcm";

    // Forearm
    QString fa_1_file_name = "FA_DICOM.dcm";
    QByteArray fa_1 = FileUtils::readFile(
        test->forearmMeasurement->m_forearmDicomFile.fileInfo.absoluteFilePath());

    QJsonObject testJson = m_test->toJsonObject();
    QJsonObject sessionObj = m_session->getJsonObject();
    QJsonObject metadata = m_test->getMetaData().toJsonObject();

    QJsonObject files = {};
    files.insert(wb_1_file_name.replace(QRegExp(".dcm"), ""),
                 test->wholeBodyMeasurement->m_wholeBody1.size);
    files.insert(wb_2_file_name.replace(QRegExp(".dcm"), ""),
                 test->wholeBodyMeasurement->m_wholeBody2.size);
    files.insert(sp_1_file_name.replace(QRegExp(".dcm"), ""),
                 test->apSpineMeasurement->m_apSpineFile.size);
    files.insert(iva_ot_file_name.replace(QRegExp(".dcm"), ""),
                 test->ivaImagingMeasurement->m_dicomOtFile.size);
    files.insert(iva_pr_file_name.replace(QRegExp(".dcm"), ""),
                 test->ivaImagingMeasurement->m_dicomPrFile.size);
    files.insert(iva_measure_file_name.replace(QRegExp(".dcm"), ""),
                 test->ivaImagingMeasurement->m_dicomMeasureFile.size);
    files.insert(fa_1_file_name.replace(QRegExp(".dcm"), ""),
                 test->forearmMeasurement->m_forearmDicomFile.size);

    qDebug() << files;

    testJson.insert("session", sessionObj);
    testJson.insert("files", files);

    QJsonObject responseJson = {};
    responseJson.insert("value", testJson);
    qDebug() << responseJson;

    QJsonDocument jsonDoc(responseJson);
    QByteArray serializedData = jsonDoc.toJson();

    QString host = CypressSettings::getPineHost();
    QString endpoint = CypressSettings::getPineEndpoint();

    sendHTTPSRequest("PATCH",
                     host + endpoint + QString::number(answer_id),
                     "application/json",
                     serializedData);
    sendHTTPSRequest("PATCH",
                     host + endpoint + QString::number(answer_id) + "?filename=" + wb_1_file_name
                         + ".dcm",
                     "application/octet-stream",
                     wb_1);
    sendHTTPSRequest("PATCH",
                     host + endpoint + QString::number(answer_id) + "?filename=" + wb_2_file_name
                         + ".dcm",
                     "application/octet-stream",
                     wb_2);
    sendHTTPSRequest("PATCH",
                     host + endpoint + QString::number(answer_id) + "?filename=" + sp_1_file_name
                         + ".dcm",
                     "application/octet-stream",
                     sp_1);
    sendHTTPSRequest("PATCH",
                     host + endpoint + QString::number(answer_id) + "?filename=" + iva_ot_file_name
                         + ".dcm",
                     "application/octet-stream",
                     iva_ot);
    sendHTTPSRequest("PATCH",
                     host + endpoint + QString::number(answer_id) + "?filename=" + iva_pr_file_name
                         + ".dcm",
                     "application/octet-stream",
                     iva_pr);
    sendHTTPSRequest("PATCH",
                     host + endpoint + QString::number(answer_id)
                         + "?filename=" + iva_measure_file_name + ".dcm",
                     "application/octet-stream",
                     iva_measure);
    sendHTTPSRequest("PATCH",
                     host + endpoint + QString::number(answer_id) + "?filename=" + fa_1_file_name
                         + ".dcm",
                     "application/octet-stream",
                     fa_1);

    emit success("");
}

QMap<QString, QVariant> DXAManager::retrieveDeviceData()
{
    return QVariantMap();
}


QMap<QString, QVariant> DXAManager::extractScanAnalysisData()
{
    return QVariantMap();
}


QMap<QString, QVariant> DXAManager::computeTandZScores()
{
    return QVariantMap();
}


QVariantMap DXAManager::getParticipantData()
{
    return QVariantMap();
}

// Set up device
bool DXAManager::setUp()
{
    return true;
}

bool DXAManager::cleanUp()
{
    return true;
}

bool DXAManager::clearData()
{
    return true;
}
