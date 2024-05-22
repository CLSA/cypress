#include "forearm_measurement.h"
#include "auxiliary/file_utils.h"

#include "dcmtk/dcmdata/dcuid.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcmetinf.h"

#include <QJsonObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QException>

ForearmMeasurement::ForearmMeasurement()
{
}

Side ForearmMeasurement::getSide() {
    return m_side;
}

quint8 ForearmMeasurement::getScanType() {
    return m_side == Side::LEFT ? 6 : 7;
}

QString ForearmMeasurement::getName() {
    return m_side == Side::RIGHT ? "R_FA" : "L_FA";
}

QString ForearmMeasurement::getBodyPartName() {
    return "ARM";
}

QString ForearmMeasurement::getRefType() {
    return "R";
}

QString ForearmMeasurement::getRefSource() {
    return "Hologic";
}

bool ForearmMeasurement::hasAllNeededFiles() const
{
    return m_hasDicomFile;
}

bool ForearmMeasurement::isValid() const
{
    return m_hasDicomFile;
}

bool ForearmMeasurement::isValidDicomFile(DicomFile file)
{
    if (file.bodyPartExamined != "ARM" || (file.laterality != "R" && file.laterality != "L"))
        return false;

    DcmFileFormat loadedFileFormat;
    if (!loadedFileFormat.loadFile(file.absFilePath.toStdString().c_str()).good())
        return false;

    OFString value = "";
    DcmDataset* dataset = loadedFileFormat.getDataset();

    OFString modality = "OT";
    OFString bodyPartExamined = "ARM";
    OFString imageAndFluoroscopyAreaDoseProduct = "";
    OFString patientOrientation = "";
    OFString bitsAllocated = "8";
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
    if (value != "L" && value != "R") return false;

    dataset->findAndGetOFString(DCM_SamplesPerPixel, value);
    if (value != samplesPerPixel) return false;

    loadedFileFormat.getMetaInfo()->findAndGetOFString(DCM_MediaStorageSOPClassUID, value);
    if (value != mediaStorageSOPClassUID) return false;

    return true;
}

QString ForearmMeasurement::toString() const
{
    return "";
};

void ForearmMeasurement::addDicomFile(DicomFile file)
{
    if (file.laterality == "L")
        m_side = Side::LEFT;
    else if (file.laterality == "R")
        m_side = Side::RIGHT;
    else {
        qCritical() << "ForearmMeasurement::addDicomFile: forearm file does not have a laterality";
        throw QException();
    }

    file.name = "FA_DICOM";
    file.size = FileUtils::getHumanReadableFileSize(file.absFilePath);

    setAttribute("NAME", 				file.name);
    setAttribute("SIZE", 				file.size);
    setAttribute("PATIENT_ID", 			file.patientId);
    setAttribute("FILE_PATH", 			file.absFilePath);
    setAttribute("STUDY_ID", 			file.studyId);
    setAttribute("MEDIA_STORAGE_UID", 	file.mediaStorageUID);

    m_dicomFile = file;
    m_hasDicomFile = true;
}

void ForearmMeasurement::getScanData(const QSqlDatabase &db, const QString &patientKey, const QString &scanId) {
    qInfo() << "ForearmMeasurement::getScanData";

    QSqlQuery query(db);

    query.prepare("SELECT * FROM Forearm WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    query.bindValue(":patientKey", patientKey);
    query.bindValue(":scanId", scanId);

    if (!query.exec()) {
        qDebug() << "forearmQuery: " << patientKey << scanId;
        throw QException();
    }

    if (!query.first()) {
        qDebug() << "Forearm: could not find a result for" << patientKey << scanId;
        return;
    }

    setAttribute("PHYSICIAN_COMMENT", query.value("PHYSICIAN_COMMENT").toString());

    setAttribute("R_13_AREA",     query.value("R_13_AREA").toDouble());
    setAttribute("R_13_BMC",      query.value("R_13_BMC").toDouble());
    setAttribute("R_13_BMD",      query.value("R_13_BMD").toDouble());

    setAttribute("R_MID_AREA",    query.value("R_MID_AREA").toDouble());
    setAttribute("R_MID_BMC",     query.value("R_MID_BMC").toDouble());
    setAttribute("R_MID_BMD",     query.value("R_MID_BMD").toDouble());

    setAttribute("R_UD_AREA",     query.value("R_UD_AREA").toDouble());
    setAttribute("R_UD_BMC",      query.value("R_UD_BMC").toDouble());
    setAttribute("R_UD_BMD",      query.value("R_UD_BMD").toDouble());

    setAttribute("U_13_AREA",     query.value("U_13_AREA").toDouble());
    setAttribute("U_13_BMC",      query.value("U_13_BMC").toDouble());
    setAttribute("U_13_BMD",      query.value("U_13_BMD").toDouble());

    setAttribute("U_MID_AREA",    query.value("U_MID_AREA").toDouble());
    setAttribute("U_MID_BMC",     query.value("U_MID_BMC").toDouble());
    setAttribute("U_MID_BMD",     query.value("U_MID_BMD").toDouble());

    setAttribute("U_UD_AREA",     query.value("U_UD_AREA").toDouble());
    setAttribute("U_UD_BMC",      query.value("U_UD_BMC").toDouble());
    setAttribute("U_UD_BMD",      query.value("U_UD_BMD").toDouble());

    setAttribute("RTOT_AREA",     query.value("RTOT_AREA").toDouble());
    setAttribute("RTOT_BMC",      query.value("RTOT_BMC").toDouble());
    setAttribute("RTOT_BMD",      query.value("RTOT_BMD").toDouble());

    setAttribute("UTOT_AREA",     query.value("UTOT_AREA").toDouble());
    setAttribute("UTOT_BMC",      query.value("UTOT_BMC").toDouble());
    setAttribute("UTOT_BMD",      query.value("UTOT_BMD").toDouble());

    setAttribute("RU13TOT_AREA",  query.value("RU13TOT_AREA").toDouble());
    setAttribute("RU13TOT_BMC",   query.value("RU13TOT_BMC").toDouble());
    setAttribute("RU13TOT_BMD",   query.value("RU13TOT_BMD").toDouble());

    setAttribute("RUMIDTOT_AREA", query.value("RUMIDTOT_AREA").toDouble());
    setAttribute("RUMIDTOT_BMC",  query.value("RUMIDTOT_BMC").toDouble());
    setAttribute("RUMIDTOT_BMD",  query.value("RUMIDTOT_BMD").toDouble());

    setAttribute("RUUDTOT_AREA",  query.value("RUUDTOT_AREA").toDouble());
    setAttribute("RUUDTOT_BMC",   query.value("RUUDTOT_BMC").toDouble());
    setAttribute("RUUDTOT_BMD",   query.value("RUUDTOT_BMD").toDouble());

    setAttribute("RUUDTOT_BMD",   query.value("RUUDTOT_BMD").toDouble());
    setAttribute("RUTOT_BMC",     query.value("RUTOT_BMC").toDouble());
    setAttribute("RUTOT_BMD",     query.value("RUTOT_BMD").toDouble());

    setAttribute("ROI_TYPE",      query.value("ROI_TYPE").toDouble());
    setAttribute("ROI_WIDTH",     query.value("ROI_WIDTH").toDouble());
    setAttribute("ROI_HEIGHT",    query.value("ROI_HEIGHT").toDouble());

    setAttribute("ARM_LENGTH",    query.value("ARM_LENGTH").toDouble());
}
