#include "ap_spine_test.h"

#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcuid.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcmetinf.h"


const QList<QString> ApSpineTest::m_inputKeyList =
{
};

const QList<QString> ApSpineTest::m_outputKeyList =
{
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
};

ApSpineTest::ApSpineTest()
{
    qDebug();
}

Side ApSpineTest::getSide()
{
   return Side::BOTH;
}

quint8 ApSpineTest::getScanType()
{
   return 0;
}

QString ApSpineTest::getName()
{
   return "AP Spine";
}

QString ApSpineTest::getBodyPartName()
{
   return "Spine";
}

QString ApSpineTest::getRefType()
{
   return "Ref Type";
}

QString ApSpineTest::getRefSource()
{
   return "Ref Source";
}

QString ApSpineTest::toString() const
{
   return "";
}

bool ApSpineTest::isValid() const
{
   return false;
}

bool ApSpineTest::isValidDicom(DcmFileFormat& loadedFileFormat) const
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
}

QJsonObject ApSpineTest::toJsonObject() const
{
    return QJsonObject();
}

void ApSpineTest::reset()
{

}


