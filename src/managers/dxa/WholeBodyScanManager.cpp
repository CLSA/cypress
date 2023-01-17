#include "WholeBodyScanManager.h"
#include <QVariant>
#include <QString>
#include <QJsonObject>

#include "auxiliary/JsonSettings.h"

#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcuid.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcmetinf.h"

WholeBodyScanManager::WholeBodyScanManager(QWidget *parent)
    : DXAManager{parent}
{
}

QVariantMap WholeBodyScanManager::retrieveDeviceData()
{
    return QVariantMap();
}

void WholeBodyScanManager::finish()
{
    DXAManager::finish();

    QJsonObject results = JsonSettings::readJsonFromFile(
        "C:/work/clsa/cypress/src/tests/fixtures/dxa/whole_body/output.json"
    );

    bool ok = sendResultsToPine(results);
    if (!ok)
    {
        qDebug() << "Could not send results to Pine";
    }
}

QString WholeBodyScanManager::getName()
{
    return "WholeBody";
}

QString WholeBodyScanManager::getBodyPartName()
{
   return "";
}

Side WholeBodyScanManager::getSide()
{
    return m_test.side;
}

quint8 WholeBodyScanManager::getScanType()
{
    //switch(m_test.side) {
    //    case LEFT:
    //        return 61;
    //    case RIGHT:
    //        return 71;
    //}

    return -1;
}

QString WholeBodyScanManager::getRefType()
{
    return "";
}

QString WholeBodyScanManager::getRefSource()
{
   return "Hologic";
}

bool WholeBodyScanManager::validateDicomFile(DcmFileFormat &file)
{
    const OFString modality = "OT";
    const OFString bodyPartExamined = "";
    const OFString imageAndFluoroscopyAreaDoseProduct = "";
    const OFString patientOrientation = "";
    const OFString bitsAllocated = "8";
    const OFString photometricInterpretation = "RGB";
    const OFString pixelSpacing = "";
    const OFString samplesPerPixel = "3";
    const OFString mediaStorageSOPClassUID = UID_SecondaryCaptureImageStorage;

    bool valid = true;
    OFString value = "";
    DcmDataset* dataset = file.getDataset();

    //const QString dicom1Name = getResultPrefix() + "_DICOM_1";
    //const QString dicom2Name = getResultPrefix() + "_DICOM_2";

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

    file.getMetaInfo()->tagExists(DCM_MediaStorageSOPClassUID);
    if (!valid)
    {
        return false;
    }

    file.getMetaInfo()->findAndGetOFString(DCM_MediaStorageSOPClassUID, value);
    if (value != mediaStorageSOPClassUID)
    {
        return false;
    }

    return valid;
}

QVariantMap WholeBodyScanManager::extractScanAnalysisData()
{
    // make connection to PatScan.mdb
    // query data

    return {{"", ""}};
}

QVariantMap WholeBodyScanManager::computeTandZScores()
{
    return QMap<QString, QVariant> {{}};
}

void WholeBodyScanManager::clearData()
{

}
