#include "iva_imaging_measurement.h"
#include "auxiliary/file_utils.h"

#include "dcmtk/dcmdata/dcuid.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcmetinf.h"

#include <QJsonObject>

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

void IVAImagingMeasurement::simulate()
{
    QJsonObject json = readJsonFile("C:/Users/Anthony/Documents/GitHub/cypress/src/tests/fixtures/dxa/iva.json");

    QStringList keys = json.keys();
    for (const QString &key : keys)
    {
        QJsonValue value = json.value(key);
        setAttribute(key, value);
    }
}

bool IVAImagingMeasurement::isValid() const
{
    return hasMeasureFile && hasOtFile && hasPrFile;
};

bool IVAImagingMeasurement::isValidDicomFile(DicomFile file) const
{
    DcmFileFormat loadedFileFormat;
    if (!loadedFileFormat.loadFile(file.absFilePath.toStdString().c_str()).good())
        return false;

    return isDicomMeasureFile(loadedFileFormat) || isDicomOTFile(loadedFileFormat)
           || isDicomPRFile(loadedFileFormat);
}

void IVAImagingMeasurement::addDicomFile(DicomFile file)
{
    DcmFileFormat loadedFileFormat;
    if (!loadedFileFormat.loadFile(file.absFilePath.toStdString().c_str()).good()) {
        return;
    }

    setAttribute("PATIENT_ID", file.patientId);
    setAttribute("STUDY_ID", file.studyId);
    setAttribute("MEDIA_STORAGE_UID", file.mediaStorageUID);
    setAttribute("FILEPATH", file.absFilePath);

    if (isDicomMeasureFile(loadedFileFormat)) {
        qDebug() << "adding IVA measure";
        m_dicomMeasureFile = file;
        m_dicomMeasureFile.name = "SEL_DICOM_MEASURE";
        m_dicomMeasureFile.size = FileUtils::getHumanReadableFileSize(
            m_dicomMeasureFile.absFilePath);

        hasMeasureFile = true;
    }

    else if (isDicomPRFile(loadedFileFormat)) {
        qDebug() << "adding IVA PR";
        m_dicomPrFile = file;
        m_dicomPrFile.name = "SEL_DICOM_PR";
        m_dicomPrFile.size = FileUtils::getHumanReadableFileSize(m_dicomPrFile.absFilePath);

        hasPrFile = true;
    }

    else if (isDicomOTFile(loadedFileFormat)) {
        qDebug() << "adding IVA OT";
        m_dicomOtFile = file;
        m_dicomOtFile.name = "SEL_DICOM_OT";
        m_dicomOtFile.size = FileUtils::getHumanReadableFileSize(m_dicomOtFile.absFilePath);

        hasOtFile = true;
    }
}

Side IVAImagingMeasurement::getSide() {
    return Side::BOTH;
}

quint8 IVAImagingMeasurement::getScanType() {
    return 29;
}

QString IVAImagingMeasurement::getName() {
    return "DEL";
}

QString IVAImagingMeasurement::getBodyPartName() {
    return "LSPINE";
}

QString IVAImagingMeasurement::getRefType() {
    return "L";
}

QString IVAImagingMeasurement::getRefSource() {
    return "NHANES";
}

bool IVAImagingMeasurement::isDicomMeasureFile(DcmFileFormat &file) const
{
    OFString value = "";
    DcmDataset *dataset = file.getDataset();

    OFString modality = "OT";
    OFString bodyPartExamined = "LSPINE";
    OFString imageAndFluoroscopyAreaDoseProduct = "";
    OFString patientOrientation = "";
    OFString bitsAllocated = "16";
    OFString photometricInterpretation = "MONOCHROME2";
    OFString pixelSpacing = "";
    OFString samplesPerPixel = "1";
    OFString mediaStorageSOPClassUID = UID_SecondaryCaptureImageStorage;

    if (!dataset->tagExistsWithValue(DCM_Modality))
        return false;
    if (!dataset->tagExistsWithValue(DCM_BodyPartExamined))
        return false;
    if (!dataset->tagExists(DCM_ImageAndFluoroscopyAreaDoseProduct))
        return false;
    if (!dataset->tagExists(DCM_PatientOrientation))
        return false;
    if (!dataset->tagExistsWithValue(DCM_BitsAllocated))
        return false;
    if (!dataset->tagExistsWithValue(DCM_PhotometricInterpretation))
        return false;
    if (!dataset->tagExists(DCM_PixelSpacing))
        return false;
    if (!dataset->tagExistsWithValue(DCM_SamplesPerPixel))
        return false;
    if (!file.getMetaInfo()->tagExists(DCM_MediaStorageSOPClassUID))
        return false;

    dataset->findAndGetOFString(DCM_Modality, value);
    if (value != modality)
        return false;

    dataset->findAndGetOFString(DCM_BodyPartExamined, value);
    if (value != bodyPartExamined)
        return false;

    dataset->findAndGetOFString(DCM_BitsAllocated, value);
    if (value != bitsAllocated)
        return false;

    dataset->findAndGetOFString(DCM_PhotometricInterpretation, value);
    if (value != photometricInterpretation)
        return false;

    dataset->findAndGetOFString(DCM_SamplesPerPixel, value);
    if (value != samplesPerPixel)
        return false;

    file.getMetaInfo()->findAndGetOFString(DCM_MediaStorageSOPClassUID, value);
    if (value != mediaStorageSOPClassUID)
        return false;

    return true;
}

bool IVAImagingMeasurement::isDicomPRFile(DcmFileFormat &file) const
{
    OFString value = "";
    DcmDataset *dataset = file.getDataset();

    OFString modality = "PR";
    OFString mediaStorageSOPClassUID = UID_GrayscaleSoftcopyPresentationStateStorage;

    if (!dataset->tagExistsWithValue(DCM_Modality))
        return false;
    if (!file.getMetaInfo()->tagExists(DCM_MediaStorageSOPClassUID))
        return false;

    dataset->findAndGetOFString(DCM_Modality, value);
    if (value != modality)
        return false;

    file.getMetaInfo()->findAndGetOFString(DCM_MediaStorageSOPClassUID, value);
    if (value != mediaStorageSOPClassUID)
        return false;

    return true;
}

bool IVAImagingMeasurement::isDicomOTFile(DcmFileFormat &file) const
{
    OFString value = "";
    DcmDataset *dataset = file.getDataset();

    OFString modality = "OT";
    OFString bitsAllocated = "8";
    OFString photometricInterpretation = "MONOCHROME2";
    OFString samplesPerPixel = "1";
    OFString mediaStorageSOPClassUID = UID_SecondaryCaptureImageStorage;

    // P/R file
    if (!dataset->tagExistsWithValue(DCM_Modality))
        return false;
    if (!dataset->tagExistsWithValue(DCM_BitsAllocated))
        return false;
    if (!dataset->tagExistsWithValue(DCM_PhotometricInterpretation))
        return false;
    if (!dataset->tagExistsWithValue(DCM_SamplesPerPixel))
        return false;
    if (!file.getMetaInfo()->tagExists(DCM_MediaStorageSOPClassUID))
        return false;

    dataset->findAndGetOFString(DCM_Modality, value);
    if (value != modality)
        return false;

    dataset->findAndGetOFString(DCM_BitsAllocated, value);
    if (value != bitsAllocated)
        return false;

    dataset->findAndGetOFString(DCM_PhotometricInterpretation, value);
    if (value != photometricInterpretation)
        return false;

    dataset->findAndGetOFString(DCM_SamplesPerPixel, value);
    if (value != samplesPerPixel)
        return false;

    file.getMetaInfo()->findAndGetOFString(DCM_MediaStorageSOPClassUID, value);
    if (value != mediaStorageSOPClassUID)
        return false;

    return true;
}

bool IVAImagingMeasurement::hasAllNeededFiles() const
{
    return hasMeasureFile && hasOtFile && hasPrFile;
}

void IVAImagingMeasurement::getScanData(const QSqlDatabase &db,
                                 const QString &patientKey,
                                     const QString &scanId) {
    Q_UNUSED(db)
    Q_UNUSED(patientKey)
    Q_UNUSED(scanId)
}
