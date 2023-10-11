#include "ap_spine_measurement.h"
#include <QJsonObject>
#include <QStringList>

#include "dcmtk/dcmdata/dcuid.h"
#include "dcmtk/dcmdata/dcdeftag.h"

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


ApSpineMeasurement::ApSpineMeasurement()
{
    m_mdb_keys = QStringList({
        "L1_INCLUDED",      // boolean
        "L1_AREA", 		    // double
        "L1_BMC",           // double
        "L1_BMD", 		    // double

        "L1_INCLUDED",      // boolean
        "L1_AREA", 		    // double
        "L1_BMC", 		    // double
        "L1_BMD", 		    // double

        "L1_INCLUDED",  	// boolean
        "L1_AREA",      	// double
        "L1_BMC",       	// double
        "L1_BMD",       	// double

        "L1_INCLUDED",  	// boolean
        "L1_AREA", 			// double
        "L1_BMC", 			// double
        "L1_BMD", 			// double

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
};

bool ApSpineMeasurement::isValid() const
{
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

    if (!dataset->tagExistsWithValue(DCM_Modality)) return false;
    if (!dataset->tagExists(DCM_BodyPartExamined)) return false;
    if (!dataset->tagExists(DCM_ImageAndFluoroscopyAreaDoseProduct)) return false;
    if (!dataset->tagExists(DCM_PatientOrientation)) return false;
    if (!dataset->tagExistsWithValue(DCM_BitsAllocated)) return false;
    if (!dataset->tagExistsWithValue(DCM_PhotometricInterpretation)) return false;
    if (!dataset->tagExists(DCM_PixelSpacing)) return false;
    if (!dataset->tagExistsWithValue(DCM_SamplesPerPixel)) return false;
    if (!loadedFileFormat.getMetaInfo()->tagExists(DCM_MediaStorageSOPClassUID)) return false;

    dataset->findAndGetOFString(DCM_Modality, value);
    if (value != modality) return false;

    dataset->findAndGetOFString(DCM_BitsAllocated, value);
    if (value != bitsAllocated) return false;

    dataset->findAndGetOFString(DCM_PhotometricInterpretation, value);
    if (value != photometricInterpretation) return false;

    dataset->findAndGetOFString(DCM_SamplesPerPixel, value);
    if (value != samplesPerPixel) return false;

    loadedFileFormat.getMetaInfo()->findAndGetOFString(DCM_MediaStorageSOPClassUID, value);
    if (value != mediaStorageSOPClassUID) return false;

    return true;
};

QJsonObject ApSpineMeasurement::toJsonObject() const
{
   return QJsonObject();
};
