#include <QMap>
#include <QVariant>
#include <QString>
#include <QSql>
#include <QProcess>
#include <QSettings>
#include <QException>

#include "dcmtk/dcmdata/dcfilefo.h"
#include "managers/dxa/DXAManager.h"
#include "auxiliary/JsonSettings.h"

#include "CypressApplication.h"


DXAManager::DXAManager(QJsonObject inputData) /* : m_dicomWatcher(QDir::currentPath())*/
{
    m_inputData = jsonObjectToVariantMap(inputData);
    qDebug() << "DXA inputData: " << inputData;
}

DXAManager::~DXAManager()
{
    //m_dicomSCP->stop();
    //delete m_dicomSCP;
}


bool DXAManager::isAvailable()
{
    return false;
}


// what the manager does in response to the main application
// window invoking its run method
//
void DXAManager::start()
{

}

// retrieve a measurement from the device
//
void DXAManager::measure()
{
    qDebug() << "measure";
    if (CypressApplication::getInstance().isSimulation()) {
        sendJsonData("C:/work/clsa/cypress/src/tests/fixtures/dxa/output.json");
        return;
    }

    //QVariantMap participantData = getParticipantData();
    //if (validatedDicomFiles.empty()) return;

    //QVariantMap scanAnalysisData = extractScanAnalysisData();
    //if (scanAnalysisData.isEmpty()) return;

    //QVariantMap scores = computeTandZScores();
    //if (scores.isEmpty()) return;

    //scanAnalysisJson = QJsonObject::fromVariantMap(scanAnalysisData);
    //scoresJson = QJsonObject::fromVariantMap(scanAnalysisData);

    //QJsonObject results = JsonSettings::readJsonFromFile(
    //    "C:/work/clsa/cypress/src/tests/fixtures/ultrasound/output.json"
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
    //endDicomServer();
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

