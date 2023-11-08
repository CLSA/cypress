#include "ap_spine_measurement.h"

#include "dcmtk/dcmdata/dcuid.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcmetinf.h"

#include <QJsonObject>
#include <QStringList>

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
    return isValidDicomFile(apSpineFile);
}

bool ApSpineMeasurement::isValidDicomFile(DicomFile file) const
{
    DcmFileFormat loadedFileFormat;
    if (!loadedFileFormat.loadFile(file.fileInfo.absoluteFilePath().toStdString().c_str()).good())
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
    {
        qDebug() << "modality";
        return false;
    }

    if (!dataset->tagExists(DCM_BodyPartExamined))
    {
        qDebug() << "body part";
        return false;
    }

    if (!dataset->tagExists(DCM_ImageAndFluoroscopyAreaDoseProduct))
    {
        qDebug() << "dose product";
        return false;
    }

    if (!dataset->tagExists(DCM_PatientOrientation))
    {
        qDebug() << "orientation";
        return false;
    }

    if (!dataset->tagExistsWithValue(DCM_BitsAllocated))
    {
        return false;
    }

    if (!dataset->tagExistsWithValue(DCM_PhotometricInterpretation))
    {
        return false;
    }

    if (!dataset->tagExists(DCM_PixelSpacing))
    {
        qDebug() << "pixel spacing";
        return false;
    }

    if (!dataset->tagExistsWithValue(DCM_SamplesPerPixel))
    {
        qDebug() << "samples per pixel";
        return false;
    }

    if (!loadedFileFormat.getMetaInfo()->tagExists(DCM_MediaStorageSOPClassUID))
    {
        qDebug() << "media storage";
        return false;
    }

    dataset->findAndGetOFString(DCM_Modality, value);
    if (value != modality)
    {
        qDebug() << "modality 2";
        return false;
    }

    dataset->findAndGetOFString(DCM_BitsAllocated, value);
    if (value != bitsAllocated)
    {
        qDebug() << "bits allocated";
        return false;
    }

    dataset->findAndGetOFString(DCM_PhotometricInterpretation, value);
    if (value != photometricInterpretation)
    {
        qDebug() << "photometric";
        return false;
    }

    dataset->findAndGetOFString(DCM_SamplesPerPixel, value);
    if (value != samplesPerPixel)
    {
        qDebug() << "samples per pixel";
        return false;
    }

    loadedFileFormat.getMetaInfo()->findAndGetOFString(DCM_MediaStorageSOPClassUID, value);
    if (value != mediaStorageSOPClassUID)
    {
        qDebug() << "media storage sop class";
        return false;
    }

    return true;
}

void ApSpineMeasurement::addDicomFile(DicomFile file)
{
    apSpineFile = file;
}
