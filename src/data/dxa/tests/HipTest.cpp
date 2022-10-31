#include "HipTest.h"
#include <QJsonObject>


// HIP RESULT SET EXTRACTOR
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

    // Hip HSA
    "NN_CSA",				// double
    "NN_BMD", 				// double
    "NN_CSMI", 				// double
    "NN_WIDTH",			    // double
    "NN_ED",				// double
    "NN_ACT",				// double
    "NN_PCD",				// double
    "NN_CMP",				// double
    "NN_SECT_MOD",			// double
    "NN_BR",				// double
    "IT_BMD",				// double
    "IT_CSA",				// double
    "IT_CSMI",				// double
    "IT_WIDTH",				// double
    "IT_ED",				// double
    "IT_ACT",				// double
    "IT_PCD",				// double
    "IT_CMP",				// double
    "IT_SECT_MOD",			// double
    "IT_BR", 				// double
    "FS_BMD",				// double
    "FS_CSA",				// double
    "FS_CSMI",				// double
    "FS_WIDTH",				// double
    "FS_ED",				// double
    "FS_ACT",				// double
    "FS_PCD",				// double
    "FS_CMP",				// double
    "FS_SECT_MOD",			// double
    "FS_BR",				// double
    "SHAFT_NECK_ANGLE",		// double
};

HipTest::HipTest()
{


};

// String representation for debug and GUI display purposes
//
QString HipTest::toString() const
{
    return "";
};

// derived classes should at minimum check
// - number of measurements = expected number of measurements
// - meta data received matches on expected keys
// - attributes received matched on expected keys
//
bool HipTest::isValid() const
{
    return false;
};

// String keys are converted to snake_case
//
QJsonObject HipTest::toJsonObject() const
{
    return QJsonObject();
};

// default is to reset both meta and measurement data
//
void HipTest::reset()
{
    m_metaData.reset();
    m_measurementList.clear();
}
