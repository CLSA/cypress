#include "cypress_application.h"
#include "cypress_session.h"

#include "../../auxiliary/Utilities.h"
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
}

DxaHipManager::~DxaHipManager()
{
    //m_dicomSCP->stop();
    //delete m_dicomSCP;
}


bool DxaHipManager::isAvailable()
{
    return true;
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
    emit started(m_test.get());
    emit canMeasure();
}

// retrieve a measurement from the device
//
void DxaHipManager::measure()
{
    m_test->reset();

    if (Cypress::getInstance().isSimulation())
        m_test->simulate();

    emit measured(m_test.get());
    emit canFinish();
}

// implementation of final clean up of device after disconnecting and all
// data has been retrieved and processed by any upstream classes
//



void DxaHipManager::finish()
{
    int answer_id = m_session->getAnswerId();
    QString barcode = m_session->getBarcode();

    // Hip
    QString hip_1_file_name = "HIP_DICOM.dcm";
    QByteArray hip_1 = FileUtils::readFileIntoByteArray("C:/Users/Anthony/Downloads/DEXA_SIM/HIP/HIP_DICOM.dcm");
    int hip_1_size = hip_1.size();

    QJsonObject testJson = m_test->toJsonObject();
    QJsonObject sessionObj = m_session->getJsonObject();
    QJsonObject metadata = m_test->getMetaData().toJsonObject();

    QJsonObject files = {};
    files.insert(hip_1_file_name.replace(QRegExp(".dcm"), ""), Utilities::bytesToSize(hip_1_size));

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
                     host + endpoint + QString::number(answer_id) + "?filename=" + hip_1_file_name + ".dcm",
                     "application/octet-stream",
                     hip_1);

    emit success("");
}

bool DxaHipManager::isCompleteDicom(DcmFileFormat &file)
{
    Q_UNUSED(file)
    return false;
}

bool DxaHipManager::isCorrectDicom(DcmFileFormat &file)
{
    Q_UNUSED(file)
    return true;
}

bool DxaHipManager::validateDicomFile(DcmFileFormat &loadedFileFormat)
{
    bool isComplete = isCorrectDicom(loadedFileFormat);
    bool isCorrect = isCompleteDicom(loadedFileFormat);

    return isComplete && isCorrect;
}

void DxaHipManager::dicomFilesReceived(QStringList paths)
{
    QVariantMap deviceData = retrieveDeviceData();
    if (deviceData.isEmpty()) {
        qInfo() << "No device data..";
        return;
    }

    validatedDicomFiles = getValidatedFiles(paths);
    if (validatedDicomFiles.isEmpty()) {
        qInfo() << "No valid dicom files received..";
        return;
    }
}

QList<DcmFileFormat> DxaHipManager::getValidatedFiles(QStringList filePaths)
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

bool DxaHipManager::startDicomServer()
{
    //return m_dicomSCP->start();
    return true;
}

bool DxaHipManager::endDicomServer()
{
    //return m_dicomSCP->stop();
    return true;
}

void DxaHipManager::dicomServerExitNormal()
{

}

void DxaHipManager::dicomServerExitCrash()
{

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

void DxaHipManager::setInputData(const QVariantMap& inputData)
{
    Q_UNUSED(inputData)
}