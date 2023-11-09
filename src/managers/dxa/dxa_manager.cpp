#include "cypress_application.h"
#include "cypress_session.h"

#include "dicom/dcm_recv.h"
#include "data/dxa/tests/dxa_test.h"
#include "managers/dxa/dxa_manager.h"
#include "auxiliary/Utilities.h"

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

    QDir workingDir = QDir::current();
    QString workingDirPath = workingDir.absolutePath() + "/";

    const QString executablePath = workingDirPath + CypressSettings::getInstance().readSetting("dxa/dicom/executable").toString();
    const QString aeTitle = CypressSettings::getInstance().readSetting("dxa/dicom/aeTitle").toString();
    const QString host = CypressSettings::getInstance().readSetting("dxa/dicom/host").toString();
    const QString port = CypressSettings::getInstance().readSetting("dxa/dicom/port").toString();

    const QString storageDirPath = workingDirPath + CypressSettings::getInstance().readSetting("dxa/dicom/storagePath").toString();
    const QString logConfigPath = workingDirPath + CypressSettings::getInstance().readSetting("dxa/dicom/log_config").toString();
    const QString ascConfigPath
        = workingDirPath
          + CypressSettings::getInstance().readSetting("dxa/dicom/asc_config").toString();

    m_dicomServer.reset(new DcmRecv(executablePath, ascConfigPath, storageDirPath, aeTitle, port));
    connect(m_dicomServer.get(),
            &DcmRecv::dicomFilesReceived,
            this,
            &DXAManager::dicomFilesReceived);

    m_networkFileCopier.reset(new SMBFileCopier());
    connect(m_networkFileCopier.get(),
            &SMBFileCopier::fileCopied,
            this,
            &DXAManager::copiedDatabaseFile);
    m_dicomServer->start();
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

bool DXAManager::isAvailable()
{
    return true;
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
    emit started(m_test.get());
}

// retrieve a measurement from the device
//
void DXAManager::measure()
{
    if (Cypress::getInstance().isSimulation()) {
        m_test->reset();
        m_test->simulate();

        emit measured(m_test.get());
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
        emit measured(test);
        emit canFinish();
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
    QByteArray wb_1 = FileUtils::readFileIntoByteArray(
        test->wholeBodyMeasurement->m_wholeBody1.fileInfo.absoluteFilePath());
    QByteArray wb_2 = FileUtils::readFileIntoByteArray(
        test->wholeBodyMeasurement->m_wholeBody2.fileInfo.absoluteFilePath());

    QString wb_1_file_name = test->wholeBodyMeasurement->m_wholeBody1.name + ".dcm";
    QString wb_2_file_name = test->wholeBodyMeasurement->m_wholeBody2.name + ".dcm";

    QByteArray sp_1 = FileUtils::readFileIntoByteArray(
        test->apSpineMeasurement->m_apSpineFile.fileInfo.absoluteFilePath());

    // AP Lumbar Spine
    QString sp_1_file_name = test->apSpineMeasurement->m_apSpineFile.name + ".dcm";

    QByteArray iva_ot = FileUtils::readFileIntoByteArray(
        test->ivaImagingMeasurement->m_dicomOtFile.fileInfo.absoluteFilePath());
    QByteArray iva_pr = FileUtils::readFileIntoByteArray(
        test->ivaImagingMeasurement->m_dicomPrFile.fileInfo.absoluteFilePath());
    QByteArray iva_measure = FileUtils::readFileIntoByteArray(
        test->ivaImagingMeasurement->m_dicomMeasureFile.fileInfo.absoluteFilePath());

    // IVA
    QString iva_ot_file_name = test->ivaImagingMeasurement->m_dicomOtFile.name + ".dcm";
    QString iva_pr_file_name = test->ivaImagingMeasurement->m_dicomPrFile.name + ".dcm";
    QString iva_measure_file_name = test->ivaImagingMeasurement->m_dicomMeasureFile.name + ".dcm";

    // Forearm
    QString fa_1_file_name = "FA_DICOM.dcm";
    QByteArray fa_1 = FileUtils::readFileIntoByteArray(
        "C:/Users/Anthony/Downloads/DEXA_SIM/FA/FA_DICOM.dcm");

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

    QString host = CypressSettings::getInstance().getPineHost();
    QString endpoint = CypressSettings::getInstance().getPineEndpoint();

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

bool DXAManager::isCompleteDicom(DcmFileFormat &file)
{
    Q_UNUSED(file)
    return false;
}

bool DXAManager::isCorrectDicom(DcmFileFormat &file)
{
    Q_UNUSED(file)
    return true;
}

bool DXAManager::validateDicomFile(DcmFileFormat &loadedFileFormat)
{
    bool isComplete = isCorrectDicom(loadedFileFormat);
    bool isCorrect = isCompleteDicom(loadedFileFormat);

    return isComplete && isCorrect;
}

QList<DcmFileFormat> DXAManager::getValidatedFiles(QStringList filePaths)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");
    QList<DcmFileFormat> validatedDicomFiles;

    DcmFileFormat fileFormat;
    QList<QString>::const_iterator i;

    try {
        for (i = filePaths.begin(); i != filePaths.end(); ++i)
        {
            const QString filePath = *i;
            QString fileName = settings.value("dicom/out_dir", "").toString() + "/" + filePath.toStdString().c_str();
            fileName = fileName.replace("/", "\\");

            OFCondition status = fileFormat.loadFile(fileName.toStdString().c_str());
            if (status.good())
            {
                bool isValid = validateDicomFile(fileFormat);
                if (isValid)
                {
                    qInfo() << "DICOM File is valid: " << isValid;
                    validatedDicomFiles.append(fileFormat);
                }
            }
            else
            {
              qInfo() << "Error: cannot read DICOM file (" << status.text() << ")" << endl;
            }
        }
    }

    catch (QException &e)
    {
        qInfo() << e.what();
    }

    return validatedDicomFiles;
}

void DXAManager::dicomServerExitNormal()
{

}

void DXAManager::dicomServerExitCrash()
{

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

void DXAManager::setInputData(const QVariantMap& inputData)
{
    Q_UNUSED(inputData)
}

