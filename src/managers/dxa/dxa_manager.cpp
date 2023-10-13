

//#include "dcmtk/dcmdata/dcfilefo.h"

#include "data/dxa/tests/dxa_test.h"
#include "managers/dxa/dxa_manager.h"

#include "../../auxiliary/file_utils.h"

#include "cypress_session.h"

//#include "auxiliary/json_settings.h"
//#include "cypress_application.h"

#include <QMap>
#include <QVariant>
#include <QString>
#include <QSql>
#include <QProcess>
#include <QSettings>
#include <QException>
#include <QJsonDocument>


DXAManager::DXAManager(const CypressSession& session) /* : m_dicomWatcher(QDir::currentPath())*/
    : ManagerBase(session)
{
    qDebug() << "DXAManager::New";
    m_test.reset(new DXATest);
}

DXAManager::~DXAManager()
{
    qDebug() << "DXAManager::Destroy";
    //m_dicomSCP->stop();
    //delete m_dicomSCP;
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
    emit canMeasure();
}

// retrieve a measurement from the device
//
void DXAManager::measure()
{
    m_test->reset();
    m_test->simulate();

    emit measured(m_test.get());

    //if (m_test->isValid())
    //{
    emit canFinish();
    //}

    //qDebug() << "measure"; //if (Cypress::getInstance().isSimulation()) {
    //    sendResultsToPine("C:/dev/clsa/cypress/src/tests/fixtures/dxa/output.json");
    //    sendFileToPine("C:/dev/clsa/cypress/src/tests/fixtures/dxa/whole_body.dcm", "whole_body.dcm");
    //    sendFileToPine("C:/dev/clsa/cypress/src/tests/fixtures/dxa/whole_body2.dcm", "whole_body2.dcm");
    //    return;
    //}

    //QVariantMap participantData = getParticipantData();
    //if (validatedDicomFiles.empty()) return;

    //QVariantMap scanAnalysisData = extractScanAnalysisData();
    //if (scanAnalysisData.isEmpty()) return;

    //QVariantMap scores = computeTandZScores();
    //if (scores.isEmpty()) return;

    //scanAnalysisJson = QJsonObject::fromVariantMap(scanAnalysisData);
    //scoresJson = QJsonObject::fromVariantMap(scanAnalysisData);

    //QJsonObject results = JsonSettings::readJsonFromFile(
    //    "C:/dev/clsa/cypress/src/tests/fixtures/ultrasound/output.json"
    //);

    //results["cypress_session"] = m_uuid;
    //results["answer_id"] = m_answerId;
    //results["barcode"] = m_barcode;
    //results["interviewer"] = m_interviewer;

    //if (results.empty()) return;

    //bool ok = sendResultsToPine(results);
    //if (!ok)
    //{
    //    qDebug() << "Could not send results to Pine";
    //}
}

// implementation of final clean up of device after disconnecting and all
// data has been retrieved and processed by any upstream classes
//
void DXAManager::finish()
{
    int answer_id = m_session.getAnswerId();
    QString barcode = m_session.getBarcode();

    // Whole body
    QByteArray wb_1 = FileUtils::readFileIntoByteArray("C:/Users/Anthony/Downloads/DEXA_SIM/WB/WB_DICOM_1.dcm");
    QByteArray wb_2 = FileUtils::readFileIntoByteArray("C:/Users/Anthony/Downloads/DEXA_SIM/WB/WB_DICOM_2.dcm");

    QString wb_1_file_name = "WB_DICOM_1.dcm";
    QString wb_2_file_name = "WB_DICOM_2.dcm";

    int wb_1_size = wb_1.size();
    int wb_2_size = wb_2.size();

    // AP Lumbar Spine
    QString sp_1_file_name = "SP_DICOM_1.dcm";

    QByteArray sp_1 = FileUtils::readFileIntoByteArray("C:/Users/Anthony/Downloads/DEXA_SIM/SP/SP_DICOM_1.dcm");

    int sp_1_size = sp_1.size();

    // IVA
    QString iva_ot_file_name = "SEL_DICOM_OT.dcm";
    QString iva_pr_file_name = "SEL_DICOM_PR.dcm";
    QString iva_measure_file_name = "SEL_DICOM_MEASURE.dcm";

    QByteArray iva_ot = FileUtils::readFileIntoByteArray("C:/Users/Anthony/Downloads/DEXA_SIM/IVA/SEL_DICOM_OT.dcm");
    QByteArray iva_pr = FileUtils::readFileIntoByteArray("C:/Users/Anthony/Downloads/DEXA_SIM/IVA/SEL_DICOM_PR.dcm");
    QByteArray iva_measure = FileUtils::readFileIntoByteArray("C:/Users/Anthony/Downloads/DEXA_SIM/IVA/SEL_DICOM_MEASURE.dcm");

    int iva_ot_size = iva_ot.size();
    int iva_pr_size = iva_pr.size();
    int iva_measure_size = iva_measure.size();

    // Hip
    QString hip_1_file_name = "HIP_DICOM.dcm";

    QByteArray hip_1 = FileUtils::readFileIntoByteArray("C:/Users/Anthony/Downloads/DEXA_SIM/HIP/HIP_DICOM.dcm");

    int hip_1_size = hip_1.size();

    // Forearm
    QString fa_1_file_name = "FA_DICOM.dcm";

    QByteArray fa_1 = FileUtils::readFileIntoByteArray("C:/Users/Anthony/Downloads/DEXA_SIM/FA/FA_DICOM.dcm");

    int fa_1_size = fa_1.size();

    QJsonObject testJson = m_test->toJsonObject();
    QJsonObject sessionObj = m_session.getJsonObject();
    QJsonObject metadata = m_test->getMetaData().toJsonObject();

    QJsonObject files = {};
    files.insert(wb_1_file_name, wb_1_size);
    files.insert(wb_2_file_name, wb_2_size);
    files.insert(sp_1_file_name, sp_1_size);
    files.insert(iva_ot_file_name, iva_ot_size);
    files.insert(iva_pr_file_name, iva_pr_size);
    files.insert(iva_measure_file_name, iva_measure_size);
    files.insert(hip_1_file_name, hip_1_size);
    files.insert(fa_1_file_name, fa_1_size);

    testJson.insert("session", sessionObj);
    testJson.insert("files", files);

    QJsonObject responseJson = {};
    responseJson.insert("value", testJson);
    qDebug() << responseJson;

    QJsonDocument jsonDoc(responseJson);
    QByteArray serializedData = jsonDoc.toJson();

    sendHTTPSRequest("PATCH",
        "https://blueberry.clsa-elcv.ca/qa/pine/api/answer/" + QString::number(answer_id),
        "application/json",
        serializedData
    );

    sendHTTPSRequest("PATCH", "https://blueberry.clsa-elcv.ca/qa/pine/api/answer/" + QString::number(answer_id) + "?filename=" + wb_1_file_name, "application/octet-stream", wb_1);
    sendHTTPSRequest("PATCH", "https://blueberry.clsa-elcv.ca/qa/pine/api/answer/" + QString::number(answer_id) + "?filename=" + wb_2_file_name, "application/octet-stream", wb_2);
    sendHTTPSRequest("PATCH", "https://blueberry.clsa-elcv.ca/qa/pine/api/answer/" + QString::number(answer_id) + "?filename=" + sp_1_file_name, "application/octet-stream", sp_1);
    sendHTTPSRequest("PATCH", "https://blueberry.clsa-elcv.ca/qa/pine/api/answer/" + QString::number(answer_id) + "?filename=" + iva_ot_file_name, "application/octet-stream", iva_ot);
    sendHTTPSRequest("PATCH", "https://blueberry.clsa-elcv.ca/qa/pine/api/answer/" + QString::number(answer_id) + "?filename=" + iva_pr_file_name, "application/octet-stream", iva_pr);
    sendHTTPSRequest("PATCH", "https://blueberry.clsa-elcv.ca/qa/pine/api/answer/" + QString::number(answer_id) + "?filename=" + iva_measure_file_name, "application/octet-stream", iva_measure);
    sendHTTPSRequest("PATCH", "https://blueberry.clsa-elcv.ca/qa/pine/api/answer/" + QString::number(answer_id) + "?filename=" + hip_1_file_name, "application/octet-stream", hip_1);
    sendHTTPSRequest("PATCH", "https://blueberry.clsa-elcv.ca/qa/pine/api/answer/" + QString::number(answer_id) + "?filename=" + fa_1_file_name, "application/octet-stream", fa_1);

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

void DXAManager::dicomFilesReceived(QStringList paths)
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

bool DXAManager::startDicomServer()
{
    //return m_dicomSCP->start();
    return true;
}

bool DXAManager::endDicomServer()
{
    //return m_dicomSCP->stop();
    return true;
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

