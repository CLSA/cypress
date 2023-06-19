#include "iva_imaging_test.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcuid.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcmetinf.h"

const QList<QString> IVAImagingTest::m_outputKeyList = {

};

const QList<QString> IVAImagingTest::m_inputKeyList = {

};

IVAImagingTest::IVAImagingTest()
{

}

Side IVAImagingTest::getSide() {
    return Side::BOTH;
}

quint8 IVAImagingTest::getScanType() {
    return 0;
}

QString IVAImagingTest::getName() {
    return "ForearmTest";
}

QString IVAImagingTest::getBodyPartName() {
    return "Forearm";
}

QString IVAImagingTest::getRefType() {
    return 0;
}

QString IVAImagingTest::getRefSource() {
    return "ref";
}

bool IVAImagingTest::isValid() const
{
    return false;
}

bool IVAImagingTest::isValidDicom(DcmFileFormat &loadedFileFormat) const
{
    OFString value = "";
    DcmDataset* dataset = loadedFileFormat.getDataset();

    OFString modality = "OT";
    OFString bodyPartExamined = "LSPINE";
    OFString imageAndFluoroscopyAreaDoseProduct = "";
    OFString patientOrientation = "";
    OFString bitsAllocated = "16";
    OFString photometricInterpretation = "MONOCHROME2";
    OFString pixelSpacing = "";
    OFString samplesPerPixel = "1";
    OFString mediaStorageSOPClassUID = UID_SecondaryCaptureImageStorage;

    if (!dataset->tagExistsWithValue(DCM_Modality)) return false;
    if (!dataset->tagExistsWithValue(DCM_BodyPartExamined)) return false;
    if (!dataset->tagExists(DCM_ImageAndFluoroscopyAreaDoseProduct)) return false;
    if (!dataset->tagExists(DCM_PatientOrientation)) return false;
    if (!dataset->tagExistsWithValue(DCM_BitsAllocated)) return false;
    if (!dataset->tagExistsWithValue(DCM_PhotometricInterpretation)) return false;
    if (!dataset->tagExists(DCM_PixelSpacing)) return false;
    if (!dataset->tagExistsWithValue(DCM_SamplesPerPixel)) return false;
    if (!loadedFileFormat.getMetaInfo()->tagExists(DCM_MediaStorageSOPClassUID)) return false;

    dataset->findAndGetOFString(DCM_Modality, value);
    if (value != modality) return false;

    dataset->findAndGetOFString(DCM_BodyPartExamined, value);
    if (value != bodyPartExamined) return false;

    dataset->findAndGetOFString(DCM_BitsAllocated, value);
    if (value != bitsAllocated) return false;

    dataset->findAndGetOFString(DCM_PhotometricInterpretation, value);
    if (value != photometricInterpretation) return false;

    dataset->findAndGetOFString(DCM_SamplesPerPixel, value);
    if (value != samplesPerPixel) return false;

    loadedFileFormat.getMetaInfo()->findAndGetOFString(DCM_MediaStorageSOPClassUID, value);
    if (value != mediaStorageSOPClassUID) return false;

    return true;
}

void IVAImagingTest::reset()
{
    m_metaData.reset();
    m_measurementList.clear();
}

QString IVAImagingTest::toString() const
{
   return "";
}

QJsonObject IVAImagingTest::toJsonObject() const
{
    return QJsonObject();
}




