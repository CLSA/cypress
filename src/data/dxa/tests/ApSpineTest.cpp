#include "ApSpineTest.h"

const QList<QString> ApSpineTest::m_inputKeyList = {
};

const QList<QString> ApSpineTest::m_outputKeyList = {
    "NO_REGIONS", 	    // long
    "STARTING_REGION",  // long

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
    "PHYSICIAN_COMMENT" // string
};

ApSpineTest::ApSpineTest()
{
    qDebug();
}

QString ApSpineTest::toString() const
{
   return "";
}

bool ApSpineTest::isValid() const
{
   return false;
}

QJsonObject ApSpineTest::toJsonObject() const
{
    return QJsonObject();
}

void ApSpineTest::reset()
{

}


