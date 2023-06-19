#include <QJsonObject>

#include "hip_test.h"

#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcuid.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcmetinf.h"

const QList<QString> HipTest::m_inputKeyList = {

};

const QList<QString> HipTest::m_outputKeyList = {
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

HipTest::HipTest()
{

}


Side HipTest::getSide() {
    return Side::BOTH;
}

quint8 HipTest::getScanType() {
    return 0;
}

QString HipTest::getName() {
    return "ForearmTest";
}

QString HipTest::getBodyPartName() {
    return "Forearm";
}

QString HipTest::getRefType() {
    return 0;
}

QString HipTest::getRefSource() {
    return "ref";
}

bool HipTest::isValid() const
{
    return false;
};

bool HipTest::isValidDicom(DcmFileFormat &loadedFileFormat) const
{
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
    {
        return false;
    }

    dataset->findAndGetOFString(DCM_Modality, value);
    if (value != modality)
    {
        return false;
    }

    // BodyPartExamined == ""
    valid = dataset->tagExistsWithValue(DCM_BodyPartExamined);
    if (!valid)
    {
        return false;
    }

    dataset->findAndGetOFString(DCM_BodyPartExamined, value);
    if (value != bodyPartExamined)
    {
        return false;
    }

    valid = dataset->tagExists(DCM_ImageAndFluoroscopyAreaDoseProduct);
    if (!valid)
    {
        return false;
    }

    valid = dataset->tagExists(DCM_PatientOrientation);
    if (!valid)
    {
        return false;
    }

    valid = dataset->tagExistsWithValue(DCM_BitsAllocated);
    if (!valid)
    {
        return false;
    }
    dataset->findAndGetOFString(DCM_BitsAllocated, value);
    if (value != bitsAllocated)
    {
        return false;
    }

    valid = dataset->tagExistsWithValue(DCM_PhotometricInterpretation);
    if (!valid)
    {
        return false;
    }
    dataset->findAndGetOFString(DCM_PhotometricInterpretation, value);
    if (value != photometricInterpretation)
    {
        return false;
    }

    valid = dataset->tagExistsWithValue(DCM_Laterality);
    if (!valid)
    {
        return false;
    }
    dataset->findAndGetOFString(DCM_Laterality, value);
    if (value != laterality)
    {
        return false;
    }

    valid = dataset->tagExists(DCM_PixelSpacing);
    if (!valid)
    {
        return false;
    }

    valid = dataset->tagExistsWithValue(DCM_SamplesPerPixel);
    if (!valid)
    {
        return false;
    }
    dataset->findAndGetOFString(DCM_SamplesPerPixel, value);
    if (value != samplesPerPixel)
    {
        return false;
    }

    loadedFileFormat.getMetaInfo()->tagExists(DCM_MediaStorageSOPClassUID);
    if (!valid)
    {
        return false;
    }

    loadedFileFormat.getMetaInfo()->findAndGetOFString(DCM_MediaStorageSOPClassUID, value);
    if (value != mediaStorageSOPClassUID)
    {
        return false;
    }

    return valid;
}

QJsonObject HipTest::toJsonObject() const
{
    return QJsonObject();
}

QString HipTest::toString() const
{
    return "";
}

void HipTest::reset()
{
    m_metaData.reset();
    m_measurementList.clear();
}
