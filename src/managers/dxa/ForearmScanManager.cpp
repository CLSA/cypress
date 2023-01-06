#include "ForearmScanManager.h"
#include <QMap>
#include <QVariant>
#include <QString>

#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcuid.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcmetinf.h"

ForearmScanManager::ForearmScanManager(QWidget *parent)
    : DXAManager{parent}
{
}

QVariantMap ForearmScanManager::retrieveDeviceData()
{
   return QVariantMap();
}

void ForearmScanManager::start()
{
    qDebug() << "ForearmScanManager::start";
    startDicomServer();
}

void ForearmScanManager::measure()
{

}

void ForearmScanManager::finish()
{
    qDebug() << "ForearmScanManager::end";
    endDicomServer();
}

QString ForearmScanManager::getName()
{
    switch (m_test.side)
    {
        case LEFT:
            return "L_FA";
        case RIGHT:
            return "R_FA";
    }

    return "R_FA";
}

QString ForearmScanManager::getBodyPartName()
{
   return "ARM";
}

Side ForearmScanManager::getSide()
{
    return m_test.side;
}

quint8 ForearmScanManager::getScanType()
{
    switch(m_test.side) {
        case LEFT:
            return 61;
        case RIGHT:
            return 71;
    }

    return 71;
}

QString ForearmScanManager::getRefType()
{
    return "R";
}

QString ForearmScanManager::getRefSource()
{
   return "Hologic";
}

bool ForearmScanManager::validateDicomFile(DcmFileFormat loadedFileFormat)
{
    bool valid = true;
    OFString value = "";
    DcmDataset* dataset = loadedFileFormat.getDataset();

    OFString modality = "OT";
    OFString bodyPartExamined = "ARM";
    OFString imageAndFluoroscopyAreaDoseProduct = "";
    OFString patientOrientation = "";
    OFString bitsAllocated = "8";
    OFString laterality = "L";
    OFString photometricInterpretation = "RGB";
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


QVariantMap ForearmScanManager::extractScanAnalysisData()
{
    // make connection to PatScan.mdb
    // query data

    return QVariantMap {{"", ""}};
}

QVariantMap ForearmScanManager::computeTandZScores()
{
    return QVariantMap {{"", ""}};
}

QJsonObject ForearmScanManager::toJsonObject() const
{
    return QJsonObject();
}

void ForearmScanManager::initializeModel()
{

}

void ForearmScanManager::updateModel()
{

}

void ForearmScanManager::setInputData(const QVariantMap& inputData)
{

}

void ForearmScanManager::clearData()
{

}

