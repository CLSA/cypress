#include "whole_body_measurement.h"

#include "dcmtk/dcmdata/dcuid.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcmetinf.h"

#include <QJsonObject>

// { "WBTOT_BMD",      "NULL" },

const QList<QString> m_outputKeyList = {
    "WBTOT_AREA",
    "WBTOT_BMC",
    "WBTOT_BMD",

    "SUBTOT_AREA",
    "SUBTOT_BMC",
    "SUBTOT_BMD",

    "HEAD_AREA",
    "HEAD_BMC",
    "HEAD_BMD",

    "LARM_AREA",
    "LARM_BMC",
    "LARM_BMD",

    "RARM_AREA",
    "RARM_BMC",
    "RARM_BMD",

    "LRIB_AREA",
    "LRIB_BMC",
    "LRIB_BMD",

    "RRIB_AREA",
    "RRIB_BMC",
    "RRIB_BMD",

    "T_S_AREA",
    "T_S_BMC",
    "T_S_BMD",

    "L_S_AREA",
    "L_S_BMC",
    "L_S_BMD",

    "PELV_AREA",
    "PELV_BMC",
    "PELV_BMD",

    "LLEG_AREA",
    "LLEG_BMC",
    "LLEG_BMD",

    "RLEG_AREA",
    "RLEG_BMC",
    "RLEG_BMD",

    "FAT_STD",
    "LEAN_STD",
    "BRAIN_FAT",
    "WATER_LBM",

    "HEAD_FAT",
    "HEAD_LEAN",
    "HEAD_MASS",
    "HEAD_PFAT",

    "LARM_FAT",
    "LARM_LEAN",
    "LARM_MASS",
    "LARM_PFAT",

    "RARM_FAT",
    "RARM_LEAN",
    "RARM_MASS",
    "RARM_PFAT",

    "TRUNK_FAT",
    "TRUNK_LEAN",
    "TRUNK_MASS",
    "TRUNK_PFAT",

    "L_LEG_FAT",
    "L_LEG_LEAN",
    "L_LEG_MASS",
    "L_LEG_PFAT",

    "R_LEG_FAT",
    "R_LEG_LEAN",
    "R_LEG_MASS",
    "R_LEG_PFAT",

    "SUBTOT_FAT",
    "SUBTOT_LEAN",
    "SUBTOT_MASS",
    "SUBTOT_PFAT",

    "WBTOT_FAT",
    "WBTOT_LEAN",
    "WBTOT_MASS",
    "WBTOT_PFAT",

    "NET_AVG_AREA",
    "NET_AVG_BMC",
    "NET_AVG_BMD",

    "GLOBAL_AREA",
    "GLOBAL_BMC",
    "GLOBAL_BMD",

    "NO_REGIONS",

    "REG1_NAME",
    "REG1_AREA",
    "REG1_BMC",
    "REG1_BMD",

    "REG2_NAME",
    "REG2_AREA",
    "REG2_BMC",
    "REG2_BMD",

    "REG3_NAME",
    "REG3_AREA",
    "REG3_BMC",
    "REG3_BMD",

    "REG4_NAME",
    "REG4_AREA",
    "REG4_BMC",
    "REG4_BMD",

    "REG5_NAME",
    "REG5_AREA",
    "REG5_BMC",
    "REG5_BMD",

    "REG6_NAME",
    "REG6_AREA",
    "REG6_BMC",
    "REG6_BMD",

    "REG7_NAME",
    "REG7_AREA",
    "REG7_BMC",
    "REG7_BMD",

    "REG8_NAME",
    "REG8_AREA",
    "REG8_BMC",
    "REG8_BMD",

    "REG9_NAME",
    "REG9_AREA",
    "REG9_BMC",
    "REG9_BMD",

    "REG10_NAME",
    "REG10_AREA",
    "REG10_BMC",
    "REG10_BMD",

    "REG11_NAME",
    "REG11_AREA",
    "REG11_BMC",
    "REG11_BMD",

    "REG12_NAME",
    "REG12_AREA",
    "REG12_BMC",
    "REG12_BMD",

    "REG13_NAME",
    "REG13_AREA",
    "REG13_BMC",
    "REG13_BMD",

    "REG14_NAME",
    "REG14_AREA",
    "REG14_BMC",
    "REG14_BMD",

    "NET_AVG_FAT",
    "NET_AVG_LEAN",
    "NET_AVG_MASS",
    "NET_AVG_PFAT",

    "GLOBAL_FAT",
    "GLOBAL_LEAN",
    "GLOBAL_MASS",
    "GLOBAL_PFAT",

    "NO_REGIONS",

    "REG1_FAT",
    "REG1_LEAN",
    "REG1_MASS",
    "REG1_PFAT",

    "REG2_FAT",
    "REG2_LEAN",
    "REG2_MASS",
    "REG2_PFAT",

    "REG3_FAT",
    "REG3_LEAN",
    "REG3_MASS",
    "REG3_PFAT",

    "REG4_FAT",
    "REG4_LEAN",
    "REG4_MASS",
    "REG4_PFAT",

    "REG5_FAT",
    "REG5_LEAN",
    "REG5_MASS",
    "REG5_PFAT",

    "REG6_FAT",
    "REG6_LEAN",
    "REG6_MASS",
    "REG6_PFAT",

    "REG7_FAT",
    "REG7_LEAN",
    "REG7_MASS",
    "REG7_PFAT",

    "REG8_FAT",
    "REG8_LEAN",
    "REG8_MASS",
    "REG8_PFAT",

    "REG9_FAT",
    "REG9_LEAN",
    "REG9_MASS",
    "REG9_PFAT",

    "REG10_FAT",
    "REG10_LEAN",
    "REG10_MASS",
    "REG10_PFAT",

    "REG11_FAT",
    "REG11_LEAN",
    "REG11_MASS",
    "REG11_PFAT",

    "REG12_FAT",
    "REG12_LEAN",
    "REG12_MASS",
    "REG12_PFAT",

    "REG13_FAT",
    "REG13_LEAN",
    "REG13_MASS",
    "REG13_PFAT",

    "REG14_FAT",
    "REG14_LEAN",
    "REG14_MASS",
    "REG14_PFAT",

    "TISSUE_ANALYSIS_METHOD",
    "FAT_STD",
    "LEAN_STD",
    "BRAIN_FAT",
    "WATER_LBM",
    "TOTAL_PERCENT_FAT",
    "BODY_MASS_INDEX",
    "ANDROID_GYNOID_RATIO",
    "ANDROID_PERCENT_FAT",
    "GYNOID_PERCENT_FAT",
    "FAT_MASS_RATIO",
    "TRUNK_LIMB_FAT_MASS_RATIO",
    "FAT_MASS_HEIGHT_SQUARED",
    "TOTAL_FAT_MASS",
    "LEAN_MASS_HEIGHT_SQUARED",
    "APPENDAGE_LEAN_MASS_HEIGHT_2",
    "TOTAL_LEAN_MASS",
    "PHYSICIAN_COMMENT",
    "ANDROID_FAT",
    "ANDROID_LEAN",
    "GYNOID_FAT",
    "GYNOID_LEAN",
};

WholeBodyScanMeasurement::WholeBodyScanMeasurement()
{

}

QString WholeBodyScanMeasurement::toString() const
{
    return "";
};

QStringList WholeBodyScanMeasurement::toStringList(const bool& no_keys) const
{
    Q_UNUSED(no_keys)
    return QStringList {{}};
};

void WholeBodyScanMeasurement::simulate()
{
    QJsonObject json = readJsonFile("C:/Users/Anthony/Documents/GitHub/cypress/src/tests/fixtures/dxa/whole_body.json");

    QStringList keys = json.keys();
    for (const QString &key : keys)
    {
        QJsonValue value = json.value(key);
        setAttribute(key, value);
    }
}

bool WholeBodyScanMeasurement::isValid() const
{
    return false;
};


bool WholeBodyScanMeasurement::isValidDicomFile(DicomFile file) const
{
    DcmFileFormat loadedFileFormat;
    if (!loadedFileFormat.loadFile(file.fileInfo.absoluteFilePath().toStdString().c_str()).good())
        return false;

    const OFString modality = "OT";
    const OFString bodyPartExamined = "";
    const OFString imageAndFluoroscopyAreaDoseProduct = "";
    const OFString patientOrientation = "";
    const OFString bitsAllocated = "8";
    const OFString photometricInterpretation = "RGB";
    const OFString pixelSpacing = "";
    const OFString samplesPerPixel = "3";
    const OFString mediaStorageSOPClassUID = UID_SecondaryCaptureImageStorage;

    OFString value = "";
    DcmDataset* dataset = loadedFileFormat.getDataset();

    //const QString dicom1Name = getResultPrefix() + "_DICOM_1";
    //const QString dicom2Name = getResultPrefix() + "_DICOM_2";

    if (!dataset->tagExistsWithValue(DCM_Modality)) return false;
    if (!dataset->tagExists(DCM_BodyPartExamined)) return false;
    if (!dataset->tagExists(DCM_ImageAndFluoroscopyAreaDoseProduct)) return false;
    if (!dataset->tagExists(DCM_PatientOrientation)) return false;
    if (!dataset->tagExistsWithValue(DCM_BitsAllocated)) return false;
    if (!dataset->tagExists(DCM_PixelSpacing)) return false;
    if (!dataset->tagExistsWithValue(DCM_SamplesPerPixel)) return false;
    if (!loadedFileFormat.getMetaInfo()->tagExists(DCM_MediaStorageSOPClassUID)) return false;
    if (!dataset->tagExistsWithValue(DCM_PhotometricInterpretation)) return false;

    dataset->findAndGetOFString(DCM_Modality, value);
    if (value != modality) return false;

    dataset->findAndGetOFString(DCM_BitsAllocated, value);
    if (value != bitsAllocated) return false;

    dataset->findAndGetOFString(DCM_PhotometricInterpretation, value);
    if (value != photometricInterpretation) return false;

    dataset->findAndGetOFString(DCM_SamplesPerPixel, value);
    if (value != samplesPerPixel) return false;

    dataset->findAndGetOFString(DCM_BodyPartExamined, value);
    if (value != "") return false;

    loadedFileFormat.getMetaInfo()->findAndGetOFString(DCM_MediaStorageSOPClassUID, value);
    if (value != mediaStorageSOPClassUID) return false;

    return true;
}

void WholeBodyScanMeasurement::addDicomFile(DicomFile)
{
    qDebug() << "whole body add dicom file";
}

Side WholeBodyScanMeasurement::getSide() {
    return Side::BOTH;
}

quint8 WholeBodyScanMeasurement::getScanType() {
    return 0;
}

QString WholeBodyScanMeasurement::getName() {
    return "WholeBodyScan";
}

QString WholeBodyScanMeasurement::getBodyPartName() {
    return "WholeBody";
}

QString WholeBodyScanMeasurement::getRefType() {
    return 0;
}

QString WholeBodyScanMeasurement::getRefSource() {
    return "ref";
}
