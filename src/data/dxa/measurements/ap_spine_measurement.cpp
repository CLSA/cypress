#include "ap_spine_measurement.h"
#include "auxiliary/file_utils.h"

#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcmetinf.h"
#include "dcmtk/dcmdata/dcuid.h"

#include <QJsonObject>
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QException>

// { "L1_BMD",         "1..." },
// { "L2_BMD",         ".2.." },
// { "L3_BMD",         "..3." },
// { "L4_BMD",         "...4" },
// { "TOT_BMD",        "1234" },
// { "TOT_L1_BMD",     "1..." },
// { "TOT_L2_BMD",     ".2.." },
// { "TOT_L3_BMD",     "..3." },
// { "TOT_L4_BMD",     "...4" },
// { "TOT_L1L2_BMD",   "12.." },
// { "TOT_L1L3_BMD",   "1.3." },
// { "TOT_L1L4_BMD",   "1..4" },
// { "TOT_L2L3_BMD",   ".23." },
// { "TOT_L2L4_BMD",   ".2.4" },
//{ "TOT_L3L4_BMD",   "..34" },
//{ "TOT_L1L2L3_BMD", "123." },
//{ "TOT_L1L2L4_BMD", "12.4" },
//{ "TOT_L1L3L4_BMD", "1.34" },
//{ "TOT_L2L3L4_BMD", ".234" },

const QList<QString> m_outputKeyList =
{
    "L1_INCLUDED",      // boolean
    "L1_AREA", 		    // double
    "L1_BMC",           // double
    "L1_BMD", 		    // double

    "L2_INCLUDED",      // boolean
    "L2_AREA", 		    // double
    "L2_BMC", 		    // double
    "L2_BMD", 		    // double

    "L3_INCLUDED",  	// boolean
    "L3_AREA",      	// double
    "L3_BMC",       	// double
    "L3_BMD",       	// double

    "L4_INCLUDED",  	// boolean
    "L4_AREA", 			// double
    "L4_BMC", 			// double
    "L4_BMD", 			// double

    "TOT_AREA", 		// double
    "TOT_BMC", 			// double
    "TOT_BMD", 			// double
    "STD_TOT_BMD",  	// double
    "ROI_TYPE", 		// long
    "ROI_WIDTH", 		// double
    "ROI_HEIGHT", 		// double

    "NO_REGIONS", 	    // long
    "STARTING_REGION",  // long
    "PHYSICIAN_COMMENT" // string
};


ApSpineMeasurement::ApSpineMeasurement()
{
    m_mdb_keys = QStringList({
        "NO_REGIONS", 	    // long
        "STARTING_REGION",  // long

        "L1_INCLUDED",      // boolean
        "L1_AREA", 		    // double
        "L1_BMC",           // double
        "L1_BMD", 		    // double

        "L2_INCLUDED",      // boolean
        "L2_AREA", 		    // double
        "L2_BMC", 		    // double
        "L2_BMD", 		    // double

        "L3_INCLUDED",  	// boolean
        "L3_AREA",      	// double
        "L3_BMC",       	// double
        "L3_BMD",       	// double

        "L4_INCLUDED",  	// boolean
        "L4_AREA", 			// double
        "L4_BMC", 			// double
        "L4_BMD", 			// double

        "TOT_AREA", 		// double
        "TOT_BMC", 			// double
        "TOT_BMD", 			// double

        "TOT_T",
        "TOT_Z",

        "STD_TOT_BMD",  	// double

        "ROI_TYPE", 		// long
        "ROI_WIDTH", 		// double
        "ROI_HEIGHT", 		// double

        "PHYSICIAN_COMMENT" // string
    });

    ValidDCMTag modality(DCM_Modality, "OT");
    ValidDCMTag bodyPartExamined(DCM_BodyPartExamined, "LSPINE");
    ValidDCMTag samplesPerPixel(DCM_SamplesPerPixel, "3");
    ValidDCMTag photometricInterpretation(DCM_PhotometricInterpretation, "RGB");
    ValidDCMTag imageAndFluoroscopyAreaDoseProduct(DCM_ImageAndFluoroscopyAreaDoseProduct, "");
    ValidDCMTag patientOrientation(DCM_PatientOrientation, "");
    ValidDCMTag bitsAllocated(DCM_BitsAllocated, "");
    ValidDCMTag pixelSpacing(DCM_PixelSpacing, "");
    ValidDCMTag mediaStorageSOPClassUID(DCM_MediaStorageSOPClassUID, UID_SecondaryCaptureImageStorage);

    m_metaInfoTagExists.append(mediaStorageSOPClassUID);

    m_datasetTagExistsWithValue.append(modality);
    m_datasetTagExistsWithValue.append(bodyPartExamined);
    m_datasetTagExistsWithValue.append(samplesPerPixel);
    m_datasetTagExistsWithValue.append(photometricInterpretation);

    m_datasetTagExists.append(imageAndFluoroscopyAreaDoseProduct);
    m_datasetTagExists.append(patientOrientation);
    m_datasetTagExists.append(bitsAllocated);
    m_datasetTagExists.append(pixelSpacing);
    m_datasetTagExists.append(mediaStorageSOPClassUID);
}

Side ApSpineMeasurement::getSide()
{
   return Side::BOTH;
}

quint8 ApSpineMeasurement::getScanType()
{
   return 0;
}

QString ApSpineMeasurement::getName()
{
   return "AP Spine";
}

QString ApSpineMeasurement::getBodyPartName()
{
   return "Spine";
}

QString ApSpineMeasurement::getRefType()
{
   return "Ref Type";
}

QString ApSpineMeasurement::getRefSource()
{
   return "Ref Source";
}

bool ApSpineMeasurement::hasAllNeededFiles() const
{
   return m_hasApSpineFile;
}

QString ApSpineMeasurement::toString() const
{
   return "";
};

QStringList ApSpineMeasurement::toStringList(const bool& no_keys) const
{
    Q_UNUSED(no_keys)
   return QStringList {{}};
}

void ApSpineMeasurement::simulate()
{
    QJsonObject json = readJsonFile("C:/Users/Anthony/Documents/GitHub/cypress/src/tests/fixtures/dxa/spine.json");

    QStringList keys = json.keys();
    for (const QString &key : keys)
    {
        QJsonValue value = json.value(key);
        setAttribute(key, value);
    }
};

bool ApSpineMeasurement::isValid() const
{
    return isValidDicomFile(m_apSpineFile);
}

bool ApSpineMeasurement::isValidDicomFile(DicomFile file) const
{
    DcmFileFormat loadedFileFormat;
    if (!loadedFileFormat.loadFile(file.absFilePath.toStdString().c_str()).good())
        return false;

    OFString value = "";
    DcmDataset* dataset = loadedFileFormat.getDataset();

    OFString modality = "OT";
    OFString bodyPartExamined = "LSPINE";
    OFString imageAndFluoroscopyAreaDoseProduct = "";
    OFString patientOrientation = "";
    OFString bitsAllocated = "8";
    OFString photometricInterpretation = "RGB";
    OFString pixelSpacing = "";
    OFString samplesPerPixel = "3";
    OFString mediaStorageSOPClassUID = UID_SecondaryCaptureImageStorage;

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

    if (!dataset->tagExistsWithValue(DCM_PhotometricInterpretation))
        return false;

    if (!dataset->tagExists(DCM_PixelSpacing))
        return false;

    if (!dataset->tagExistsWithValue(DCM_SamplesPerPixel))
        return false;

    if (!loadedFileFormat.getMetaInfo()->tagExists(DCM_MediaStorageSOPClassUID))
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

    loadedFileFormat.getMetaInfo()->findAndGetOFString(DCM_MediaStorageSOPClassUID, value);
    if (value != mediaStorageSOPClassUID)
        return false;

    return true;
}

void ApSpineMeasurement::addDicomFile(DicomFile file)
{
    m_apSpineFile = file;
    m_apSpineFile.name = "SP_DICOM_1";
    m_apSpineFile.size = FileUtils::getHumanReadableFileSize(m_apSpineFile.absFilePath);
    m_hasApSpineFile = true;

    setAttribute("PATIENT_ID", file.patientId);
    setAttribute("FILEPATH", file.absFilePath);
    setAttribute("STUDY_ID", file.studyId);
    setAttribute("MEDIA_STORAGE_UID", file.mediaStorageUID);
    setAttribute("NAME", m_apSpineFile.name);
    setAttribute("SIZE", m_apSpineFile.size);
}

void ApSpineMeasurement::getScanData(const QSqlDatabase& db, const QString& patientKey, const QString& scanId) {
    QSqlQuery query(db);

    // Spine
    query.prepare("SELECT * FROM Spine WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId");
    query.bindValue(":patientKey", patientKey);
    query.bindValue(":scanId", scanId);

    if (!query.exec()) {
        qDebug() << "spine query failed" << query.lastError().text();
        throw QException();
    }

    if (!query.first()) {
        return;
    }

    setAttribute("PHYSICIAN_COMMENT", query.value("PHYSICIAN_COMMENT").toString());

    setAttribute("NO_REGIONS",      query.value("NO_REGIONS").toLongLong());
    setAttribute("STARTING_REGION", query.value("STARTING_REGION").toLongLong());

    setAttribute("L1_INCLUDED", query.value("L1_INCLUDED").toBool());
    setAttribute("L1_AREA",     query.value("L1_AREA").toDouble());
    setAttribute("L1_BMC",      query.value("L1_BMC").toDouble());
    setAttribute("L1_BMD",      query.value("L1_BMD").toDouble());

    setAttribute("L2_INCLUDED", query.value("L2_INCLUDED").toBool());
    setAttribute("L2_AREA",     query.value("L2_AREA").toDouble());
    setAttribute("L2_BMC",      query.value("L2_BMC").toDouble());
    setAttribute("L2_BMD",      query.value("L2_BMD").toDouble());

    setAttribute("L3_INCLUDED", query.value("L3_INCLUDED").toBool());
    setAttribute("L3_AREA",     query.value("L3_AREA").toDouble());
    setAttribute("L3_BMC",      query.value("L3_BMC").toDouble());
    setAttribute("L3_BMD",      query.value("L3_BMD").toDouble());

    setAttribute("L4_INCLUDED", query.value("L4_INCLUDED").toBool());
    setAttribute("L4_AREA",     query.value("L4_AREA").toDouble());
    setAttribute("L4_BMC",      query.value("L4_BMC").toDouble());
    setAttribute("L4_BMD",      query.value("L4_BMD").toDouble());

    setAttribute("TOT_AREA",    query.value("TOT_AREA").toDouble());
    setAttribute("TOT_BMC",     query.value("TOT_BMC").toDouble());
    setAttribute("TOT_BMD",     query.value("TOT_BMD").toDouble());
    setAttribute("STD_TOT_BMD", query.value("STD_TOT_BMD").toDouble());

    setAttribute("ROI_TYPE",    query.value("ROI_TYPE").toLongLong());
    setAttribute("ROI_WIDTH",   query.value("ROI_WIDTH").toDouble());
    setAttribute("ROI_HEIGHT",  query.value("ROI_HEIGHT").toDouble());
}
