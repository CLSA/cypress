#include "hip_measurement.h"

#include "auxiliary/Utilities.h"
#include "auxiliary/file_utils.h"

#include "dcmtk/dcmdata/dcuid.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcmetinf.h"

#include <QJsonObject>
#include <QRandomGenerator>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QException>

// hip
// { "NECK_BMD",       "1..." },
// { "TROCH_BMD",      ".2.." },
// { "INTER_BMD",      "..3." },
// { "WARDS_BMD",      "...4" },
// { "HTOT_BMD",       "123." },

const QList<QString> m_inputKeyList = {

};

const QList<QString> m_outputKeyList = {
    "TROCH_AREA",  		 // double
    "TROCH_BMC",   		 // double
    "TROCH_BMD",   		 // double
    "INTER_AREA",  		 // double
    "INTER_BMC",   		 // double
    "INTER_BMD",   		 // double
    "NECK_AREA",   		 // double
    "NECK_BMC",   		 // double
    "NECK_BMD",    		 // double
    "WARDS_AREA",  		 // double
    "WARDS_BMC",   		 // double
    "WARDS_BMD",   		 // double
    "HTOT_AREA",   		 // double
    "HTOT_BMC",    		 // double
    "HTOT_BMD",    		 // double
    "ROI_TYPE",      	 // long
    "ROI_WIDTH",   		 // double
    "ROI_HEIGHT",  	     // double
    "AXIS_LENGTH",       // double
    "PHYSICIAN_COMMENT", // string

    "NN_CSA",			 // double
    "NN_BMD", 			 // double
    "NN_CSMI", 			 // double
    "NN_WIDTH",			 // double
    "NN_ED",			 // double
    "NN_ACT",			 // double
    "NN_PCD",			 // double
    "NN_CMP",			 // double
    "NN_SECT_MOD",		 // double
    "NN_BR",			 // double
    "IT_BMD",			 // double
    "IT_CSA",			 // double
    "IT_CSMI",			 // double
    "IT_WIDTH",			 // double
    "IT_ED",			 // double
    "IT_ACT",			 // double
    "IT_PCD",			 // double
    "IT_CMP",			 // double
    "IT_SECT_MOD",		 // double
    "IT_BR", 			 // double
    "FS_BMD",			 // double
    "FS_CSA",			 // double
    "FS_CSMI",			 // double
    "FS_WIDTH",			 // double
    "FS_ED",			 // double
    "FS_ACT",			 // double
    "FS_PCD",			 // double
    "FS_CMP",			 // double
    "FS_SECT_MOD",		 // double
    "FS_BR",			 // double
    "SHAFT_NECK_ANGLE",	 // double
};

HipMeasurement::HipMeasurement(Side side): m_side { side }
{

}

QString HipMeasurement::toString() const
{
    return "";
};

QStringList HipMeasurement::toStringList(const bool& no_keys) const
{
    Q_UNUSED(no_keys)
    return QStringList {{}};
}

void HipMeasurement::simulate()
{
    QJsonObject json = readJsonFile("C:/Users/Anthony/Documents/GitHub/cypress/src/tests/fixtures/dxa/hip.json");

    QStringList keys = json.keys();
    for (const QString &key : keys)
    {
        QJsonValue value = json.value(key);
        setAttribute(key, value);
    }

    setAttribute("hip_neck_bmd", Utilities::interp(0.0, 1.0, QRandomGenerator::global()->generateDouble()), "g/cm^2");
}

bool HipMeasurement::isValid() const
{
    return m_hasHipFile;
}

bool HipMeasurement::isValidDicomFile(DicomFile file) const
{
    DcmFileFormat loadedFileFormat;
    if (!loadedFileFormat.loadFile(file.absFilePath.toStdString().c_str()).good())
        return false;

    bool valid = true;
    OFString value = "";
    DcmDataset* dataset = loadedFileFormat.getDataset();

    OFString modality = "OT";
    OFString bodyPartExamined = "HIP";
    OFString imageAndFluoroscopyAreaDoseProduct = "";
    OFString patientOrientation = "";
    OFString bitsAllocated = "8";
    OFString photometricInterpretation = "RGB";
    OFString laterality = "L";
    OFString pixelSpacing = "";
    OFString samplesPerPixel = "3";
    OFString mediaStorageSOPClassUID = UID_SecondaryCaptureImageStorage;

    // Modality == "OT"
    valid = dataset->tagExistsWithValue(DCM_Modality);
    if (!valid)
        return false;

    dataset->findAndGetOFString(DCM_Modality, value);
    if (value != modality)
        return false;

    // BodyPartExamined == ""
    valid = dataset->tagExistsWithValue(DCM_BodyPartExamined);
    if (!valid)
        return false;

    dataset->findAndGetOFString(DCM_BodyPartExamined, value);
    if (value != bodyPartExamined)
        return false;

    valid = dataset->tagExists(DCM_ImageAndFluoroscopyAreaDoseProduct);
    if (!valid)
        return false;

    valid = dataset->tagExists(DCM_PatientOrientation);
    if (!valid)
        return false;

    valid = dataset->tagExistsWithValue(DCM_BitsAllocated);
    if (!valid)
        return false;

    dataset->findAndGetOFString(DCM_BitsAllocated, value);
    if (value != bitsAllocated)
        return false;

    valid = dataset->tagExistsWithValue(DCM_PhotometricInterpretation);
    if (!valid)
        return false;

    dataset->findAndGetOFString(DCM_PhotometricInterpretation, value);
    if (value != photometricInterpretation)
        return false;

    valid = dataset->tagExistsWithValue(DCM_Laterality);
    if (!valid)
        return false;

    dataset->findAndGetOFString(DCM_Laterality, value);
    if (value != laterality)
        return false;

    valid = dataset->tagExists(DCM_PixelSpacing);
    if (!valid)
        return false;

    valid = dataset->tagExistsWithValue(DCM_SamplesPerPixel);
    if (!valid)
        return false;

    dataset->findAndGetOFString(DCM_SamplesPerPixel, value);
    if (value != samplesPerPixel)
        return false;

    loadedFileFormat.getMetaInfo()->tagExists(DCM_MediaStorageSOPClassUID);
    if (!valid)
        return false;

    loadedFileFormat.getMetaInfo()->findAndGetOFString(DCM_MediaStorageSOPClassUID, value);
    if (value != mediaStorageSOPClassUID)
        return false;

    return valid;
}

void HipMeasurement::addDicomFile(DicomFile file)
{
    qDebug() << "add hip measure";

    m_hipDicomFile = file;
    m_hipDicomFile.name = "HIP_DICOM";
    m_hipDicomFile.size = FileUtils::getHumanReadableFileSize(file.absFilePath);
    m_hasHipFile = true;

    setAttribute("PATIENT_ID", file.patientId);
    setAttribute("FILE_PATH", file.absFilePath);
    setAttribute("STUDY_ID", file.studyId);
    setAttribute("SIDE", file.laterality);
    setAttribute("MEDIA_STORAGE_UID", file.mediaStorageUID);
    setAttribute("NAME", "HIP_DICOM");
    setAttribute("SIZE", m_hipDicomFile.size);
}

Side HipMeasurement::getSide() {
    return m_side;
}

quint8 HipMeasurement::getScanType() {
    if (m_side == Side::LEFT) {
        return 2;
    }

    return 3;
}

QString HipMeasurement::getName() {
    if (m_side == Side::LEFT) {
        return "L_HIP";
    }

    return "R_HIP";
}

QString HipMeasurement::getBodyPartName() {
    return "HIP";
}

QString HipMeasurement::getRefType() {
    return "H";
}

QString HipMeasurement::getRefSource() {
    return "NHANES";
}

void HipMeasurement::getScanData(const QSqlDatabase &db,
                                 const QString &patientKey,
                                 const QString &scanId)
{
    QSqlQuery query(db);

    query.prepare("SELECT * FROM Hip WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    query.bindValue(":patientKey", patientKey);
    query.bindValue(":scanId", scanId);

    if (!query.exec()) {
        qDebug() << "hip query failed: " << query.lastError().text();
        throw QException();
    }

    if (!query.first()) {
        qDebug() << "Hip: could not find a result for" << patientKey << scanId;
        return;
    }

    setAttribute("TROCH_AREA", query.value("TROCH_AREA").toDouble());
    setAttribute("TROCH_BMC",  query.value("TROCH_BMC").toDouble());
    setAttribute("TROCH_BMD",  query.value("TROCH_BMD").toDouble());

    setAttribute("INTER_AREA", query.value("INTER_AREA").toDouble());
    setAttribute("INTER_BMC",  query.value("INTER_BMC").toDouble());
    setAttribute("INTER_BMD",  query.value("INTER_BMD").toDouble());

    setAttribute("NECK_AREA",  query.value("NECK_AREA").toDouble());
    setAttribute("NECK_BMC",   query.value("NECK_BMC").toDouble());
    setAttribute("NECK_BMD",   query.value("NECK_BMD").toDouble());

    setAttribute("WARDS_AREA", query.value("WARDS_AREA").toDouble());
    setAttribute("WARDS_BMC",  query.value("WARDS_BMC").toDouble());
    setAttribute("WARDS_BMD",  query.value("WARDS_BMD").toDouble());

    setAttribute("HTOT_AREA",  query.value("HTOT_AREA").toDouble());
    setAttribute("HTOT_BMC",   query.value("HTOT_BMC").toDouble());
    setAttribute("HTOT_BMD",   query.value("HTOT_BMD").toDouble());

    setAttribute("ROI_TYPE",   query.value("ROI_TYPE").toLongLong());
    setAttribute("ROI_WIDTH",  query.value("ROI_WIDTH").toDouble());
    setAttribute("ROI_HEIGHT", query.value("ROI_HEIGHT").toDouble());

    setAttribute("AXIS_LENGTH",       query.value("AXIS_LENGTH").toDouble());
    setAttribute("PHYSICIAN_COMMENT", query.value("PHYSICIAN_COMMENT").toString());

    query.prepare("SELECT * FROM HipHSA WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    query.bindValue(":patientKey", patientKey);
    query.bindValue(":scanId", scanId);

    if (!query.exec()) {
        qDebug() << "hipHSA query failed" << query.lastError().text();
        throw QException();
    }

    if (!query.first()) {
        qDebug() << "HipHSA: could not find a result for" << patientKey << scanId;
        return;
    }

    setAttribute("NN_BMD",      query.value("NN_BMD").toDouble());
    setAttribute("NN_CSA",      query.value("NN_CSA").toDouble());
    setAttribute("NN_CSMI",     query.value("NN_CSMI").toDouble());
    setAttribute("NN_WIDTH",    query.value("NN_WIDTH").toDouble());
    setAttribute("NN_ED",       query.value("NN_ED").toDouble());
    setAttribute("NN_ACT",      query.value("NN_ACT").toDouble());
    setAttribute("NN_PCD",      query.value("NN_PCD").toDouble());
    setAttribute("NN_CMP",      query.value("NN_CMP").toDouble());
    setAttribute("NN_SECT_MOD", query.value("NN_SECT_MOD").toDouble());
    setAttribute("NN_BR", 	  query.value("NN_BR").toDouble());

    setAttribute("IT_BMD",      query.value("IT_BMD").toDouble());
    setAttribute("IT_CSA",      query.value("IT_CSA").toDouble());
    setAttribute("IT_CSMI",     query.value("IT_CSMI").toDouble());
    setAttribute("IT_WIDTH",    query.value("IT_WIDTH").toDouble());
    setAttribute("IT_ED",       query.value("IT_ED").toDouble());
    setAttribute("IT_ACT",      query.value("IT_ACT").toDouble());
    setAttribute("IT_PCD",      query.value("IT_PCD").toDouble());
    setAttribute("IT_CMP",      query.value("IT_CMP").toDouble());
    setAttribute("IT_SECT_MOD", query.value("IT_SECT_MOD").toDouble());
    setAttribute("IT_BR",       query.value("IT_BR").toDouble());

    setAttribute("FS_BMD",      query.value("FS_BMD").toDouble());
    setAttribute("FS_CSA",      query.value("FS_CSA").toDouble());
    setAttribute("FS_CSMI",     query.value("FS_CSMI").toDouble());
    setAttribute("FS_WIDTH",    query.value("FS_WIDTH").toDouble());
    setAttribute("FS_ED", 	    query.value("FS_ED").toDouble());
    setAttribute("FS_ACT",      query.value("FS_ACT").toDouble());
    setAttribute("FS_PCD",      query.value("FS_PCD").toDouble());
    setAttribute("FS_CMP",      query.value("FS_CMP").toDouble());
    setAttribute("FS_SECT_MOD", query.value("FS_SECT_MOD").toDouble());
    setAttribute("FS_BR",       query.value("FS_BR").toDouble());

    setAttribute("SHAFT_NECK_ANGLE", query.value("SHAFT_NECK_ANGLE").toDouble());
}

bool HipMeasurement::hasAllNeededFiles() const
{
    return m_hasHipFile;
}
