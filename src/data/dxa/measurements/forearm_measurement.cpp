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

// { "RU13TOT_BMD",    "1.." },
// { "RUMIDTOT_BMD",   ".2." },
// { "RUUDTOT_BMD",    "..3" },
// { "RUTOT_BMD",      "123" },
// { "R_13_BMD",       "R.." },
// { "R_MID_BMD",      ".R." },
// { "R_UD_BMD",       "..R" },
// { "RTOT_BMD",       "RRR" },
// { "U_13_BMD",       "U.." },
// { "U_MID_BMD",      ".U." },
// { "U_UD_BMD",       "..U" },
// { "UTOT_BMD",       "UUU" },

ForearmMeasurement::ForearmMeasurement(Side side): m_side { side }
{
    m_mdb_keys = QStringList(
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
    });
}

Side ForearmMeasurement::getSide() {
    return m_side;
}

quint8 ForearmMeasurement::getScanType() {
    return 6;
}

QString ForearmMeasurement::getName() {
    if (m_side == Side::LEFT) {
        return "L_FA";
    }
    else if (m_side == Side::RIGHT) {
        return "R_FA";
    }

    return "L_FA";
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

void ForearmMeasurement::simulate()
{
    QJsonObject json = readJsonFile("C:/Users/Anthony/Documents/GitHub/cypress/src/tests/fixtures/dxa/forearm.json");

    const QStringList keys = json.keys();
    for (const QString &key : keys)
    {
        const QJsonValue value = json.value(key);
        setAttribute(key, value);
    }
}

bool ForearmMeasurement::hasAllNeededFiles() const
{
    return hasForearmFile;
}

bool ForearmMeasurement::isValid() const
{
    return hasForearmFile;
}

bool ForearmMeasurement::isValidDicomFile(DicomFile file) const
{
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

QStringList ForearmMeasurement::toStringList(const bool& no_keys) const
{
    Q_UNUSED(no_keys)
    return QStringList {{}};
}

void ForearmMeasurement::addDicomFile(DicomFile file)
{
    m_forearmDicomFile = file;
    m_forearmDicomFile.name = "FA_DICOM";
    m_forearmDicomFile.size = FileUtils::getHumanReadableFileSize(file.absFilePath);
    hasForearmFile = true;

    setAttribute("PATIENT_ID", file.patientId);
    setAttribute("FILE_PATH", file.absFilePath);
    setAttribute("STUDY_ID", file.studyId);
    setAttribute("MEDIA_STORAGE_UID", file.mediaStorageUID);
    setAttribute("NAME", m_forearmDicomFile.name);
    setAttribute("SIZE", m_forearmDicomFile.size);
}

void ForearmMeasurement::getScanData(const QSqlDatabase &db,
                                 const QString &patientKey,
                                     const QString &scanId) {
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
