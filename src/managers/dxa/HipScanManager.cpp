#include <QVariant>
#include <QString>
#include <QMap>
#include <QObject>
#include <QDebug>
#include <QSettings>

#include "managers/dxa/DXAManager.h"
#include "managers/dxa/HipScanManager.h"

#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcuid.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcmetinf.h"

HipScanManager::HipScanManager(QObject* parent)
    : DXAManager{parent}
{
    if (m_verbose)
        qDebug() << "Hip Scan Manager init";
}

void HipScanManager::start()
{
    if (m_verbose)
        qDebug() << "HipScanManager::start";

    startDicomServer();
}

void HipScanManager::measure()
{

}

void HipScanManager::finish()
{
    if (m_verbose)
        qDebug() << "HipScanManager::end";

    endDicomServer();
}

QString HipScanManager::getName()
{
    switch(m_test.side) {
        case LEFT:
            return "L_HIP";
        case RIGHT:
            return "R_HIP";
        default:
            return "";
    }
}

QString HipScanManager::getBodyPartName()
{
   return "";
}

Side HipScanManager::getSide()
{
    return m_test.side;
}

quint8 HipScanManager::getScanType()
{
    switch(m_test.side) {
        case LEFT:
            return 61;
        case RIGHT:
            return 71;
    }

    return -1;
}

QString HipScanManager::getRefType()
{
    return "";
}

QString HipScanManager::getRefSource()
{
   return "Hologic";
}

QVariantMap HipScanManager::extractScanAnalysisData()
{
    return QMap<QString, QVariant> {{}};
}

QVariantMap HipScanManager::computeTandZScores()
{
    return QMap<QString, QVariant> {{}};
}

bool HipScanManager::validateDicomFile(DcmFileFormat loadedFileFormat)
{
    bool valid = true;
    OFString value = "";
    DcmDataset* dataset = loadedFileFormat.getDataset();

    OFString modality = "OT";
    OFString bodyPartExamined = "HIP";
    OFString imageAndFluoroscopyAreaDoseProduct = "";
    OFString patientOrientation = "";
    OFString bitsAllocated = "8";
    OFString photometricInterpretation = "RGB";
    OFString laterality = "L";
    OFString pixelSpacing = "";
    OFString samplesPerPixel = "3";
    OFString mediaStorageSOPClassUID = UID_SecondaryCaptureImageStorage;

    // Modality == "OT"
    valid = dataset->tagExistsWithValue(DCM_Modality);
    if (!valid)
    {
        return false;
    }

    dataset->findAndGetOFString(DCM_Modality, value);
    if (value != modality)
    {
        return false;
    }

    // BodyPartExamined == ""
    valid = dataset->tagExistsWithValue(DCM_BodyPartExamined);
    if (!valid)
    {
        return false;
    }
    dataset->findAndGetOFString(DCM_BodyPartExamined, value);
    if (value != bodyPartExamined)
    {
        return false;
    }

    valid = dataset->tagExists(DCM_ImageAndFluoroscopyAreaDoseProduct);
    if (!valid)
    {
        return false;
    }

    valid = dataset->tagExists(DCM_PatientOrientation);
    if (!valid)
    {
        return false;
    }

    valid = dataset->tagExistsWithValue(DCM_BitsAllocated);
    if (!valid)
    {
        return false;
    }
    dataset->findAndGetOFString(DCM_BitsAllocated, value);
    if (value != bitsAllocated)
    {
        return false;
    }

    valid = dataset->tagExistsWithValue(DCM_PhotometricInterpretation);
    if (!valid)
    {
        return false;
    }
    dataset->findAndGetOFString(DCM_PhotometricInterpretation, value);
    if (value != photometricInterpretation)
    {
        return false;
    }

    valid = dataset->tagExistsWithValue(DCM_Laterality);
    if (!valid)
    {
        return false;
    }
    dataset->findAndGetOFString(DCM_Laterality, value);
    if (value != laterality)
    {
        return false;
    }

    valid = dataset->tagExists(DCM_PixelSpacing);
    if (!valid)
    {
        return false;
    }

    valid = dataset->tagExistsWithValue(DCM_SamplesPerPixel);
    if (!valid)
    {
        return false;
    }
    dataset->findAndGetOFString(DCM_SamplesPerPixel, value);
    if (value != samplesPerPixel)
    {
        return false;
    }

    loadedFileFormat.getMetaInfo()->tagExists(DCM_MediaStorageSOPClassUID);
    if (!valid)
    {
        return false;
    }

    loadedFileFormat.getMetaInfo()->findAndGetOFString(DCM_MediaStorageSOPClassUID, value);
    if (value != mediaStorageSOPClassUID)
    {
        return false;
    }

    return valid;
}

QJsonObject HipScanManager::toJsonObject() const
{
    return QJsonObject();
}

void HipScanManager::initializeModel()
{

}

void HipScanManager::updateModel()
{

}

void HipScanManager::setInputData(const QVariantMap& inputData)
{

}

void HipScanManager::clearData()
{

}


