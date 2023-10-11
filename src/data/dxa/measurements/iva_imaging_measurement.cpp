#include <QJsonObject>
#include "iva_imaging_measurement.h"

IVAImagingMeasurement::IVAImagingMeasurement()
{

}

QString IVAImagingMeasurement::toString() const
{
    return "";
};

QStringList IVAImagingMeasurement::toStringList(const bool& no_keys) const
{
    Q_UNUSED(no_keys)
    return QStringList {{}};
};

bool IVAImagingMeasurement::isValid() const
{
    return false;
};

QJsonObject IVAImagingMeasurement::toJsonObject() const
{
    return QJsonObject();
}

bool IVAImagingMeasurement::isValidDicomFile(DcmFileFormat &dicomFileFormat) const
{
    OFString value = "";
    DcmDataset* dataset = dicomFileFormat.getDataset();

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
    if (!dicomFileFormat.getMetaInfo()->tagExists(DCM_MediaStorageSOPClassUID)) return false;

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

    dicomFileFormat.getMetaInfo()->findAndGetOFString(DCM_MediaStorageSOPClassUID, value);
    if (value != mediaStorageSOPClassUID) return false;

    return true;
}

Side IVAImagingMeasurement::getSide() {
    return Side::BOTH;
}

quint8 IVAImagingMeasurement::getScanType() {
    return 0;
}

QString IVAImagingMeasurement::getName() {
    return "ForearmTest";
}

QString IVAImagingMeasurement::getBodyPartName() {
    return "Forearm";
}

QString IVAImagingMeasurement::getRefType() {
    return 0;
}

QString IVAImagingMeasurement::getRefSource() {
    return "ref";
}

