#include <QDebug>

#include "forearm_test.h"

#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcuid.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcmetinf.h"

const QList<QString> ForearmTest::m_inputKeyList = {

};

const QList<QString> ForearmTest::m_outputKeyList =
{
    "R_13_AREA",
    "R_13_BMC",
    "R_13_BMD",

    "R_MID_AREA",
    "R_MID_BMC",
    "R_MID_BMD",

    "R_UD_AREA",
    "R_UD_BMC",
    "R_UD_BMD",

    "U_13_AREA",
    "U_13_BMC",
    "U_13_BMD",

    "U_MID_AREA",
    "U_MID_BMC",
    "U_MID_BMD",

    "U_UD_AREA",
    "U_UD_BMC",
    "U_UD_BMD",

    "RTOT_AREA",
    "RTOT_BMC",
    "RTOT_BMD",

    "UTOT_AREA",
    "UTOT_BMC",
    "UTOT_BMD",

    "RU13TOT_AREA",
    "RU13TOT_BMC",
    "RU13TOT_BMD",

    "RUMIDTOT_AREA",
    "RUMIDTOT_BMC",
    "RUMIDTOT_BMD",

    "RUUDTOT_AREA",
    "RUUDTOT_BMC",
    "RUUDTOT_BMD",

    "RUTOT_AREA",
    "RUTOT_BMC",
    "RUTOT_BMD",

    "ROI_TYPE",
    "ROI_WIDTH",
    "ROI_HEIGHT",

    "ARM_LENGTH",
    "PHYSICIAN_COMMENT",
};

ForearmTest::ForearmTest()
{
}

Side ForearmTest::getSide() {
    return Side::BOTH;
}

quint8 ForearmTest::getScanType() {
    return 0;
}

QString ForearmTest::getName() {
    return "ForearmTest";
}

QString ForearmTest::getBodyPartName() {
    return "Forearm";
}

QString ForearmTest::getRefType() {
    return 0;
}

QString ForearmTest::getRefSource() {
    return "ref";
}

bool ForearmTest::isValid() const
{
    return false;
}

bool ForearmTest::isValidDicom(DcmFileFormat &loadedFileFormat) const
{
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

    if (!dataset->tagExistsWithValue(DCM_Modality)) return false;
    if (!dataset->tagExistsWithValue(DCM_BodyPartExamined)) return false;
    if (!dataset->tagExists(DCM_ImageAndFluoroscopyAreaDoseProduct)) return false;
    if (!dataset->tagExistsWithValue(DCM_PhotometricInterpretation)) return false;
    if (!dataset->tagExistsWithValue(DCM_Laterality)) return false;
    if (!dataset->tagExists(DCM_PixelSpacing)) return false;
    if (!dataset->tagExists(DCM_PatientOrientation)) return false;
    if (!dataset->tagExistsWithValue(DCM_BitsAllocated)) return false;
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

    dataset->findAndGetOFString(DCM_Laterality, value);
    if (value != laterality) return false;

    dataset->findAndGetOFString(DCM_SamplesPerPixel, value);
    if (value != samplesPerPixel) return false;

    loadedFileFormat.getMetaInfo()->findAndGetOFString(DCM_MediaStorageSOPClassUID, value);
    if (value != mediaStorageSOPClassUID) return false;

    return true;
}

void ForearmTest::reset()
{
    m_metaData.reset();
    m_measurementList.clear();
}

QString ForearmTest::toString() const
{
   return "";
}

QJsonObject ForearmTest::toJsonObject() const
{
    return QJsonObject();
}

