#include "whole_body_measurement.h"

#include "auxiliary/file_utils.h"

#include "dcmtk/dcmdata/dcuid.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcmetinf.h"

#include <QJsonObject>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QException>


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
    return hasWholeBody1File && hasWholeBody2File;
}

bool WholeBodyScanMeasurement::isValidDicomFile(DicomFile file) const
{
    DcmFileFormat loadedFileFormat;
    if (!loadedFileFormat.loadFile(file.absFilePath.toStdString().c_str()).good())
        return false;

    return isWholeBody1(loadedFileFormat) || isWholeBody2(loadedFileFormat);
}

void WholeBodyScanMeasurement::addDicomFile(DicomFile file)
{
    DcmFileFormat loadedFileFormat;
    if (!loadedFileFormat.loadFile(file.absFilePath.toStdString().c_str()).good()) {
        return;
    }

    setAttribute("patientId", file.patientId);
    setAttribute("filePath", file.absFilePath);
    setAttribute("studyId", file.studyId);
    setAttribute("mediaStorageUid", file.mediaStorageUID);

    if (isWholeBody1(loadedFileFormat)) {
        qDebug() << "adding dicom 1";

        m_wholeBody1 = file;
        m_wholeBody1.name = "WB_DICOM_1";
        m_wholeBody1.size = FileUtils::getHumanReadableFileSize(m_wholeBody1.absFilePath);

        hasWholeBody1File = true;
    }

    else if (isWholeBody2(loadedFileFormat)) {
        qDebug() << "adding dicom 2";
        m_wholeBody2 = file;
        m_wholeBody2.name = "WB_DICOM_2";
        m_wholeBody2.size = FileUtils::getHumanReadableFileSize(m_wholeBody2.absFilePath);

        hasWholeBody2File = true;
    }
}

Side WholeBodyScanMeasurement::getSide() {
    return Side::BOTH;
}

quint8 WholeBodyScanMeasurement::getScanType() {
    return 5;
}

QString WholeBodyScanMeasurement::getName() {
    return "WB";
}

QString WholeBodyScanMeasurement::getBodyPartName() {
    return "WBODY";
}

QString WholeBodyScanMeasurement::getRefType() {
    return "W";
}

QString WholeBodyScanMeasurement::getRefSource() {
    return "NHANES";
}

bool WholeBodyScanMeasurement::isWholeBody1(DcmFileFormat &file) const
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

    OFString value = "";
    DcmDataset *dataset = file.getDataset();

    if (!dataset->tagExistsWithValue(DCM_Modality))
        return false;
    if (!dataset->tagExists(DCM_BodyPartExamined))
        return false;
    if (!dataset->tagExists(DCM_ImageAndFluoroscopyAreaDoseProduct))
        return false;
    if (!dataset->tagExists(DCM_PatientOrientation))
        return false;
    if (!dataset->tagExistsWithValue(DCM_BitsAllocated))
        return false;
    if (!dataset->tagExists(DCM_PixelSpacing))
        return false;
    if (!dataset->tagExistsWithValue(DCM_SamplesPerPixel))
        return false;
    if (!file.getMetaInfo()->tagExists(DCM_MediaStorageSOPClassUID))
        return false;
    if (!dataset->tagExistsWithValue(DCM_PhotometricInterpretation))
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

    dataset->findAndGetOFString(DCM_BodyPartExamined, value);
    if (value != "")
        return false;

    file.getMetaInfo()->findAndGetOFString(DCM_MediaStorageSOPClassUID, value);
    if (value != mediaStorageSOPClassUID)
        return false;

    return true;
}

bool WholeBodyScanMeasurement::isWholeBody2(DcmFileFormat &file) const
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

    OFString value = "";
    DcmDataset *dataset = file.getDataset();

    //const QString dicom1Name = getResultPrefix() + "_DICOM_1";
    //const QString dicom2Name = getResultPrefix() + "_DICOM_2";

    if (!dataset->tagExistsWithValue(DCM_Modality))
        return false;
    if (!dataset->tagExists(DCM_BodyPartExamined))
        return false;
    if (dataset->tagExists(DCM_ImageAndFluoroscopyAreaDoseProduct))
        return false;
    if (!dataset->tagExists(DCM_PatientOrientation))
        return false;
    if (!dataset->tagExistsWithValue(DCM_BitsAllocated))
        return false;
    if (!dataset->tagExists(DCM_PixelSpacing))
        return false;
    if (!dataset->tagExistsWithValue(DCM_SamplesPerPixel))
        return false;
    if (!file.getMetaInfo()->tagExists(DCM_MediaStorageSOPClassUID))
        return false;
    if (!dataset->tagExistsWithValue(DCM_PhotometricInterpretation))
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

    dataset->findAndGetOFString(DCM_BodyPartExamined, value);
    if (value != "")
        return false;

    file.getMetaInfo()->findAndGetOFString(DCM_MediaStorageSOPClassUID, value);
    if (value != mediaStorageSOPClassUID)
        return false;

    return true;
}

bool WholeBodyScanMeasurement::hasAllNeededFiles() const
{
    return hasWholeBody1File && hasWholeBody2File;
}

void WholeBodyScanMeasurement::getScanData(const QSqlDatabase &db, const QString &patientKey,
                                           const QString &scanId) {
    // Whole body
    QSqlQuery query(db);

    query.prepare("SELECT * FROM Wbody WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    query.bindValue(":patientKey", patientKey);
    query.bindValue(":scanId", scanId);

    if (!query.exec()) {
        qDebug() << query.lastError().text();
        throw QException();
    }

    if (!query.first()) {
        return;
    }

    setAttribute("WBTOT_AREA",  query.value("WBTOT_AREA").toDouble());
    setAttribute("WBTOT_BMC",   query.value("WBTOT_BMC").toDouble());
    setAttribute("WBTOT_BMD",   query.value("WBTOT_BMD").toDouble());

    setAttribute("SUBTOT_AREA", query.value("SUBTOT_AREA").toDouble());
    setAttribute("SUBTOT_BMC",  query.value("SUBTOT_BMC").toDouble());
    setAttribute("SUBTOT_BMD",  query.value("SUBTOT_BMD").toDouble());

    setAttribute("HEAD_AREA",   query.value("HEAD_AREA").toDouble());
    setAttribute("HEAD_BMC",    query.value("HEAD_BMC").toDouble());
    setAttribute("HEAD_BMD",    query.value("HEAD_BMD").toDouble());

    setAttribute("LARM_AREA",   query.value("LARM_AREA").toDouble());
    setAttribute("LARM_BMC",    query.value("LARM_BMC").toDouble());
    setAttribute("LARM_BMD",    query.value("LARM_BMD").toDouble());

    setAttribute("RARM_AREA",   query.value("RARM_AREA").toDouble());
    setAttribute("RARM_BMC",    query.value("RARM_BMC").toDouble());
    setAttribute("RARM_BMD",    query.value("RARM_BMD").toDouble());

    setAttribute("LRIB_AREA",   query.value("LRIB_AREA").toDouble());
    setAttribute("LRIB_BMC",    query.value("LRIB_BMC").toDouble());
    setAttribute("LRIB_BMD",    query.value("LRIB_BMD").toDouble());

    setAttribute("RRIB_AREA",   query.value("RRIB_AREA").toDouble());
    setAttribute("RRIB_BMC",    query.value("RRIB_BMC").toDouble());
    setAttribute("RRIB_BMD",    query.value("RRIB_BMD").toDouble());

    setAttribute("T_S_AREA",    query.value("T_S_AREA").toDouble());
    setAttribute("T_S_BMC",     query.value("T_S_BMC").toDouble());
    setAttribute("T_S_BMD",     query.value("T_S_BMD").toDouble());
    setAttribute("L_S_AREA",    query.value("L_S_AREA").toDouble());
    setAttribute("L_S_BMC",     query.value("L_S_BMC").toDouble());
    setAttribute("L_S_BMD",     query.value("L_S_BMD").toDouble());

    setAttribute("PELV_AREA",   query.value("PELV_AREA").toDouble());
    setAttribute("PELV_BMC",    query.value("PELV_BMC").toDouble());
    setAttribute("PELV_BMD",    query.value("PELV_BMD").toDouble());

    setAttribute("LLEG_AREA",   query.value("LLEG_AREA").toDouble());
    setAttribute("LLEG_BMC",    query.value("LLEG_BMC").toDouble());
    setAttribute("LLEG_BMD",    query.value("LLEG_BMD").toDouble());

    setAttribute("RLEG_AREA",   query.value("RLEG_AREA").toDouble());
    setAttribute("RLEG_BMC",    query.value("RLEG_BMC").toDouble());
    setAttribute("RLEG_BMD",    query.value("RLEG_BMD").toDouble());

    setAttribute("PHYSICIAN_COMMENT", query.value("PHYSICIAN_COMMENT").toString());

    query.prepare("SELECT * FROM WbodyComposition WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    query.bindValue(":patientKey", patientKey);
    query.bindValue(":scanId", scanId);

    if (!query.exec()) {
        qDebug() << query.lastError().text();
        throw QException();
    }

    if (!query.first()) {
        return;
    }

    setAttribute("FAT_STD",     query.value("FAT_STD").toDouble());
    setAttribute("LEAN_STD",    query.value("LEAN_STD").toDouble());
    setAttribute("BRAIN_FAT",   query.value("BRAIN_FAT").toDouble());
    setAttribute("WATER_LBM",   query.value("WATER_LBM").toDouble());

    setAttribute("HEAD_FAT",    query.value("HEAD_FAT").toDouble());
    setAttribute("HEAD_LEAN",   query.value("HEAD_LEAN").toDouble());
    setAttribute("HEAD_MASS",   query.value("HEAD_MASS").toDouble());
    setAttribute("HEAD_PFAT",   query.value("HEAD_PFAT").toDouble());

    setAttribute("LARM_FAT",    query.value("LARM_FAT").toDouble());
    setAttribute("LARM_LEAN",   query.value("LARM_LEAN").toDouble());
    setAttribute("LARM_MASS",   query.value("LARM_MASS").toDouble());
    setAttribute("LARM_PFAT",   query.value("LARM_PFAT").toDouble());

    setAttribute("RARM_FAT",    query.value("RARM_FAT").toDouble());
    setAttribute("RARM_LEAN",   query.value("RARM_LEAN").toDouble());
    setAttribute("RARM_MASS",   query.value("RARM_MASS").toDouble());
    setAttribute("RARM_PFAT",   query.value("RARM_PFAT").toDouble());

    setAttribute("TRUNK_FAT",   query.value("TRUNK_FAT").toDouble());
    setAttribute("TRUNK_LEAN",  query.value("TRUNK_LEAN").toDouble());
    setAttribute("TRUNK_MASS",  query.value("TRUNK_MASS").toDouble());
    setAttribute("TRUNK_PFAT",  query.value("TRUNK_PFAT").toDouble());

    setAttribute("L_LEG_FAT",   query.value("L_LEG_FAT").toDouble());
    setAttribute("L_LEG_LEAN",  query.value("L_LEG_LEAN").toDouble());
    setAttribute("L_LEG_MASS",  query.value("L_LEG_MASS").toDouble());
    setAttribute("L_LEG_PFAT",  query.value("L_LEG_PFAT").toDouble());

    setAttribute("R_LEG_FAT",   query.value("R_LEG_FAT").toDouble());
    setAttribute("R_LEG_LEAN",  query.value("R_LEG_LEAN").toDouble());
    setAttribute("R_LEG_MASS",  query.value("R_LEG_MASS").toDouble());
    setAttribute("R_LEG_PFAT",  query.value("R_LEG_PFAT").toDouble());

    setAttribute("SUBTOT_FAT",  query.value("SUBTOT_FAT").toDouble());
    setAttribute("SUBTOT_LEAN", query.value("SUBTOT_LEAN").toDouble());
    setAttribute("SUBTOT_MASS", query.value("SUBTOT_MASS").toDouble());
    setAttribute("SUBTOT_PFAT", query.value("SUBTOT_PFAT").toDouble());

    setAttribute("WBTOT_FAT",   query.value("WBTOT_FAT").toDouble());
    setAttribute("WBTOT_LEAN",  query.value("WBTOT_LEAN").toDouble());
    setAttribute("WBTOT_MASS",  query.value("WBTOT_MASS").toDouble());
    setAttribute("WBTOT_PFAT",  query.value("WBTOT_PFAT").toDouble());

    setAttribute("PHYSICIAN_COMMENT", query.value("PHYSICIAN_COMMENT").toString());

    query.prepare("SELECT * FROM SubRegionBone WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    query.bindValue(":patientKey", patientKey);
    query.bindValue(":scanId", scanId);

    if (!query.exec()) {
        qDebug() << query.lastError().text();
        throw QException();
    }

    if (!query.first()) {
        return;
    }

    setAttribute("PHYSICIAN_COMMENT", query.value("PHYSICIAN_COMMENT").toString());
    setAttribute("NO_REGIONS",        query.value("NO_REGIONS").toLongLong());

    setAttribute("NET_AVG_AREA", query.value("NET_AVG_AREA").toDouble());
    setAttribute("NET_AVG_BMC",  query.value("NET_AVG_BMC").toDouble());
    setAttribute("NET_AVG_BMD",  query.value("NET_AVG_BMD").toDouble());

    setAttribute("GLOBAL_AREA",  query.value("GLOBAL_AREA").toDouble());
    setAttribute("GLOBAL_BMC",   query.value("GLOBAL_AREA").toDouble());
    setAttribute("GLOBAL_BMD",   query.value("GLOBAL_AREA").toDouble());

    setAttribute("REG1_NAME",    query.value("REG1_NAME").toString());
    setAttribute("REG1_AREA",    query.value("REG1_AREA").toDouble());
    setAttribute("REG1_BMC",     query.value("REG1_BMC").toDouble());
    setAttribute("REG1_BMD",     query.value("REG1_BMD").toDouble());

    setAttribute("REG2_NAME",    query.value("REG2_NAME").toString());
    setAttribute("REG2_AREA",    query.value("REG2_AREA").toDouble());
    setAttribute("REG2_BMC",     query.value("REG2_BMC").toDouble());
    setAttribute("REG2_BMD",     query.value("REG2_BMD").toDouble());

    setAttribute("REG3_NAME",    query.value("REG3_NAME").toString());
    setAttribute("REG3_AREA",    query.value("REG3_AREA").toDouble());
    setAttribute("REG3_BMC",     query.value("REG3_BMC").toDouble());
    setAttribute("REG3_BMD",     query.value("REG3_BMD").toDouble());

    setAttribute("REG3_NAME",    query.value("REG3_NAME").toString());
    setAttribute("REG3_AREA",    query.value("REG3_AREA").toDouble());
    setAttribute("REG3_BMC",     query.value("REG3_BMC").toDouble());
    setAttribute("REG3_BMD",     query.value("REG3_BMD").toDouble());

    setAttribute("REG4_NAME",    query.value("REG4_NAME").toString());
    setAttribute("REG4_AREA",    query.value("REG4_AREA").toDouble());
    setAttribute("REG4_BMC",     query.value("REG4_BMC").toDouble());
    setAttribute("REG4_BMD",     query.value("REG4_BMD").toDouble());

    setAttribute("REG5_NAME",    query.value("REG5_NAME").toString());
    setAttribute("REG5_AREA",    query.value("REG5_AREA").toDouble());
    setAttribute("REG5_BMC",     query.value("REG5_BMC").toDouble());
    setAttribute("REG5_BMD",     query.value("REG5_BMD").toDouble());

    setAttribute("REG6_NAME",    query.value("REG6_NAME").toString());
    setAttribute("REG6_AREA",    query.value("REG6_AREA").toDouble());
    setAttribute("REG6_BMC",     query.value("REG6_BMC").toDouble());
    setAttribute("REG6_BMD",     query.value("REG6_BMD").toDouble());

    setAttribute("REG7_NAME",    query.value("REG7_NAME").toString());
    setAttribute("REG7_AREA",    query.value("REG7_AREA").toDouble());
    setAttribute("REG7_BMC",     query.value("REG7_BMC").toDouble());
    setAttribute("REG7_BMD",     query.value("REG7_BMD").toDouble());

    setAttribute("REG8_NAME",    query.value("REG8_NAME").toString());
    setAttribute("REG8_AREA",    query.value("REG8_AREA").toDouble());
    setAttribute("REG8_BMC",     query.value("REG8_BMC").toDouble());
    setAttribute("REG8_BMD",     query.value("REG8_BMD").toDouble());

    setAttribute("REG9_NAME",    query.value("REG9_NAME").toString());
    setAttribute("REG9_AREA",    query.value("REG9_AREA").toDouble());
    setAttribute("REG9_BMC",     query.value("REG9_BMC").toDouble());
    setAttribute("REG9_BMD",     query.value("REG9_BMD").toDouble());

    setAttribute("REG10_NAME",   query.value("REG10_NAME").toString());
    setAttribute("REG10_AREA",   query.value("REG10_AREA").toDouble());
    setAttribute("REG10_BMC",    query.value("REG10_BMC").toDouble());
    setAttribute("REG10_BMD",    query.value("REG10_BMD").toDouble());

    setAttribute("REG11_NAME",   query.value("REG11_NAME").toString());
    setAttribute("REG11_AREA",   query.value("REG11_AREA").toDouble());
    setAttribute("REG11_BMC",    query.value("REG11_BMC").toDouble());
    setAttribute("REG11_BMD",    query.value("REG11_BMD").toDouble());

    setAttribute("REG12_NAME",   query.value("REG12_NAME").toString());
    setAttribute("REG12_AREA",   query.value("REG12_AREA").toDouble());
    setAttribute("REG12_BMC",    query.value("REG12_BMC").toDouble());
    setAttribute("REG12_BMD",    query.value("REG12_BMD").toDouble());

    setAttribute("REG13_NAME",   query.value("REG13_NAME").toString());
    setAttribute("REG13_AREA",   query.value("REG13_AREA").toDouble());
    setAttribute("REG13_BMC",    query.value("REG13_BMC").toDouble());
    setAttribute("REG13_BMD",    query.value("REG13_BMD").toDouble());

    setAttribute("REG14_NAME",   query.value("REG14_NAME").toString());
    setAttribute("REG14_AREA",   query.value("REG14_AREA").toDouble());
    setAttribute("REG14_BMC",    query.value("REG14_BMC").toDouble());
    setAttribute("REG14_BMD",    query.value("REG14_BMD").toDouble());

    query.prepare("SELECT * FROM SubRegionComposition WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    query.bindValue(":patientKey", patientKey);
    query.bindValue(":scanId", scanId);

    if (!query.exec()) {
        qDebug() << query.lastError().text();
        throw QException();
    }

    if (!query.first()) {
        return;
    }

    setAttribute("NET_AVG_FAT",  query.value("NET_AVG_FAT").toDouble());
    setAttribute("NET_AVG_LEAN", query.value("NET_AVG_LEAN").toDouble());
    setAttribute("NET_AVG_MASS", query.value("NET_AVG_MASS").toDouble());
    setAttribute("NET_AVG_PFAT", query.value("NET_AVG_PFAT").toDouble());

    setAttribute("GLOBAL_FAT",  query.value("GLOBAL_FAT").toDouble());
    setAttribute("GLOBAL_LEAN", query.value("GLOBAL_LEAN").toDouble());
    setAttribute("GLOBAL_MASS", query.value("GLOBAL_MASS").toDouble());
    setAttribute("GLOBAL_PFAT", query.value("GLOBAL_PFAT").toDouble());

    setAttribute("NO_REGIONS", query.value("NO_REGIONS").toLongLong());

    setAttribute("TISSUE_ANALYSIS_METHOD", query.value("TISSUE_ANALYSIS_METHOD").toInt());
    setAttribute("PHYSICIAN_COMMENT", query.value("PHYSICIAN_COMMENT").toString());

    setAttribute("REG1_NAME", query.value("REG1_NAME").toString());
    setAttribute("REG1_FAT",  query.value("REG1_FAT").toDouble());
    setAttribute("REG1_LEAN", query.value("REG1_LEAN").toDouble());
    setAttribute("REG1_MASS", query.value("REG1_MASS").toDouble());
    setAttribute("REG1_PFAT", query.value("REG1_PFAT").toDouble());

    setAttribute("REG2_NAME", query.value("REG2_NAME").toString());
    setAttribute("REG2_FAT",  query.value("REG2_FAT").toDouble());
    setAttribute("REG2_LEAN", query.value("REG2_LEAN").toDouble());
    setAttribute("REG2_MASS", query.value("REG2_MASS").toDouble());
    setAttribute("REG2_PFAT", query.value("REG2_PFAT").toDouble());

    setAttribute("REG3_NAME", query.value("REG3_NAME").toString());
    setAttribute("REG3_FAT",  query.value("REG3_FAT").toDouble());
    setAttribute("REG3_LEAN", query.value("REG3_LEAN").toDouble());
    setAttribute("REG3_MASS", query.value("REG3_MASS").toDouble());
    setAttribute("REG3_PFAT", query.value("REG3_PFAT").toDouble());

    setAttribute("REG4_NAME", query.value("REG4_NAME").toString());
    setAttribute("REG4_FAT",  query.value("REG4_FAT").toDouble());
    setAttribute("REG4_LEAN", query.value("REG4_LEAN").toDouble());
    setAttribute("REG4_MASS", query.value("REG4_MASS").toDouble());
    setAttribute("REG4_PFAT", query.value("REG4_PFAT").toDouble());

    setAttribute("REG5_NAME", query.value("REG5_NAME").toString());
    setAttribute("REG5_FAT",  query.value("REG5_FAT").toDouble());
    setAttribute("REG5_LEAN", query.value("REG5_LEAN").toDouble());
    setAttribute("REG5_MASS", query.value("REG5_MASS").toDouble());
    setAttribute("REG5_PFAT", query.value("REG5_PFAT").toDouble());

    setAttribute("REG6_NAME", query.value("REG6_NAME").toString());
    setAttribute("REG6_FAT",  query.value("REG6_FAT").toDouble());
    setAttribute("REG6_LEAN", query.value("REG6_LEAN").toDouble());
    setAttribute("REG6_MASS", query.value("REG6_MASS").toDouble());
    setAttribute("REG6_PFAT", query.value("REG6_PFAT").toDouble());

    setAttribute("REG7_NAME", query.value("REG7_NAME").toString());
    setAttribute("REG7_FAT",  query.value("REG7_FAT").toDouble());
    setAttribute("REG7_LEAN", query.value("REG7_LEAN").toDouble());
    setAttribute("REG7_MASS", query.value("REG7_MASS").toDouble());
    setAttribute("REG7_PFAT", query.value("REG7_PFAT").toDouble());

    setAttribute("REG8_NAME", query.value("REG8_NAME").toString());
    setAttribute("REG8_FAT",  query.value("REG8_FAT").toDouble());
    setAttribute("REG8_LEAN", query.value("REG8_LEAN").toDouble());
    setAttribute("REG8_MASS", query.value("REG8_MASS").toDouble());
    setAttribute("REG8_PFAT", query.value("REG8_PFAT").toDouble());

    setAttribute("REG9_NAME", query.value("REG9_NAME").toString());
    setAttribute("REG9_FAT",  query.value("REG9_FAT").toDouble());
    setAttribute("REG9_LEAN", query.value("REG9_LEAN").toDouble());
    setAttribute("REG9_MASS", query.value("REG9_MASS").toDouble());
    setAttribute("REG9_PFAT", query.value("REG9_PFAT").toDouble());

    setAttribute("REG10_NAME", query.value("REG10_NAME").toString());
    setAttribute("REG10_FAT",  query.value("REG10_FAT").toDouble());
    setAttribute("REG10_LEAN", query.value("REG10_LEAN").toDouble());
    setAttribute("REG10_MASS", query.value("REG10_MASS").toDouble());
    setAttribute("REG10_PFAT", query.value("REG10_PFAT").toDouble());

    setAttribute("REG11_NAME", query.value("REG11_NAME").toString());
    setAttribute("REG11_FAT",  query.value("REG11_FAT").toDouble());
    setAttribute("REG11_LEAN", query.value("REG11_LEAN").toDouble());
    setAttribute("REG11_MASS", query.value("REG11_MASS").toDouble());
    setAttribute("REG11_PFAT", query.value("REG11_PFAT").toDouble());

    setAttribute("REG12_NAME", query.value("REG12_NAME").toString());
    setAttribute("REG12_FAT",  query.value("REG12_FAT").toDouble());
    setAttribute("REG12_LEAN", query.value("REG12_LEAN").toDouble());
    setAttribute("REG12_MASS", query.value("REG12_MASS").toDouble());
    setAttribute("REG12_PFAT", query.value("REG12_PFAT").toDouble());

    setAttribute("REG13_NAME", query.value("REG13_NAME").toString());
    setAttribute("REG13_FAT",  query.value("REG13_FAT").toDouble());
    setAttribute("REG13_LEAN", query.value("REG13_LEAN").toDouble());
    setAttribute("REG13_MASS", query.value("REG13_MASS").toDouble());
    setAttribute("REG13_PFAT", query.value("REG13_PFAT").toDouble());

    setAttribute("REG14_NAME", query.value("REG14_NAME").toString());
    setAttribute("REG14_FAT",  query.value("REG14_FAT").toDouble());
    setAttribute("REG14_LEAN", query.value("REG14_LEAN").toDouble());
    setAttribute("REG14_MASS", query.value("REG14_MASS").toDouble());
    setAttribute("REG14_PFAT", query.value("REG14_PFAT").toDouble());

    query.prepare("SELECT * FROM ObesityIndices WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    query.bindValue(":patientKey", patientKey);
    query.bindValue(":scanId", scanId);

    if (!query.exec()) {
        qDebug() << query.lastError().text() << patientKey << scanId;
        throw QException();
    }

    if (!query.first()) {
        qDebug() << "wBodyObesityIndicesQuery: not found" << patientKey << scanId;
        return;
    }

    setAttribute("FAT_STD", query.value("FAT_STD").toDouble());
    setAttribute("LEAN_STD", query.value("LEAN_STD").toDouble());
    setAttribute("BRAIN_FAT", query.value("BRAIN_FAT").toDouble());
    setAttribute("WATER_LBM", query.value("WATER_LBM").toDouble());
    setAttribute("TOTAL_PERCENT_FAT", query.value("TOTAL_PERCENT_FAT").toDouble());
    setAttribute("BODY_MASS_INDEX", query.value("BODY_MASS_INDEX").toDouble());

    setAttribute("ANDROID_GYNOID_RATIO", query.value("ANDROID_GYNOID_RATIO").toDouble());
    setAttribute("ANDROID_PERCENT_FAT", query.value("ANDROID_PERCENT_FAT").toDouble());
    setAttribute("GYNOID_PERCENT_FAT", query.value("GYNOID_PERCENT_FAT").toDouble());

    setAttribute("FAT_MASS", query.value("FAT_MASS").toDouble());
    setAttribute("TRUNK_LIMB_FAT_MASS_RATIO", query.value("TRUNK_LIMB_FAT_MASS_RATIO").toDouble());
    setAttribute("FAT_MASS_HEIGHT_SQUARED", query.value("FAT_MASS_HEIGHT_SQUARED").toDouble());
    setAttribute("TOTAL_FAT_MASS", query.value("TOTAL_FAT_MASS").toDouble());

    setAttribute("LEAN_MASS_HEIGHT_SQUARED", query.value("LEAN_MASS_HEIGHT_SQUARED").toDouble());
    setAttribute("APPENDAGE_LEAN_MASS_HEIGHT_2", query.value("APPENDAGE_LEAN_MASS_HEIGHT_2").toDouble());
    setAttribute("TOTAL_LEAN_MASS", query.value("TOTAL_LEAN_MASS").toDouble());

    setAttribute("PHYSICIAN_COMMENT", query.value("PHYSICIAN_COMMENT").toString());

    query.prepare("SELECT * FROM AndroidGynoidComposition WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    query.bindValue(":patientKey", patientKey);
    query.bindValue(":scanId", scanId);

    if (!query.exec()) {
        qDebug() << query.lastError().text() << patientKey << scanId;
        throw QException();
    }

    if (!query.first()) {
        qDebug() << "wBodyAndroidGynoidCompositionQuery: " << patientKey << scanId;
        return;
    }

    setAttribute("ANDROID_FAT",  query.value("ANDROID_FAT").toDouble());
    setAttribute("ANDROID_LEAN", query.value("ANDROID_LEAN").toDouble());

    setAttribute("GYNOID_FAT",   query.value("GYNOID_FAT").toDouble());
    setAttribute("GYNOID_LEAN",  query.value("GYNOID_LEAN").toDouble());

    setAttribute("PHYSICIAN_COMMENT", query.value("PHYSICIAN_COMMENT").toString());
}
