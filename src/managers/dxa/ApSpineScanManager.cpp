#include <QMap>
#include <QVariant>
#include <QString>

#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcuid.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcmetinf.h"

#include "CypressApplication.h"
#include "auxiliary/JsonSettings.h"
#include "ApSpineScanManager.h"

APSpineScanManager::APSpineScanManager()
{

}


void APSpineScanManager::finish()
{
    DXAManager::finish();

    QJsonObject results = JsonSettings::readJsonFromFile(
        "C:/work/clsa/cypress/src/tests/fixtures/dxa/spine/output.json"
    );

    bool ok = sendResultsToPine(results);
    if (!ok)
    {

    }

    CypressApplication::status = Status::Waiting;
}

QVariantMap APSpineScanManager::retrieveDeviceData()
{
   return QVariantMap();
}

QString APSpineScanManager::getName()
{
    return "Spine";
}

QString APSpineScanManager::getBodyPartName()
{
   return "";
}

Side APSpineScanManager::getSide()
{
    return m_test.side;
}

quint8 APSpineScanManager::getScanType()
{
    //switch(m_test.side) {
    //    case LEFT:
    //        return 61;
    //    case RIGHT:
    //        return 71;
    //}

    return -1;
}

QString APSpineScanManager::getRefType()
{
    return "";
}

QString APSpineScanManager::getRefSource()
{
   return "Hologic";
}

bool APSpineScanManager::validateDicomFile(DcmFileFormat &loadedFileFormat)
{
    OFString value = "";
    DcmDataset* dataset = loadedFileFormat.getDataset();

    OFString modality = "OT";
    OFString bodyPartExamined = "LSPINE";
    OFString imageAndFluoroscopyAreaDoseProduct = "";
    OFString patientOrientation = "";
    OFString bitsAllocated = "8";
    OFString photometricInterpretation = "RGB";
    OFString pixelSpacing = "";
    OFString samplesPerPixel = "3";
    OFString mediaStorageSOPClassUID = UID_SecondaryCaptureImageStorage;

    bool valid = DXAManager::validateDicomFile(loadedFileFormat);
    if (!valid)
    {
        return false;
    }

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
    valid = dataset->tagExists(DCM_BodyPartExamined);
    if (!valid)
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

QVariantMap APSpineScanManager::extractScanAnalysisData()
{
    return QVariantMap {{}};
}

QVariantMap APSpineScanManager::computeTandZScores()
{
    return QVariantMap {{}};
}

void APSpineScanManager::clearData()
{

}
