#ifndef FOREARMTEST_H
#define FOREARMTEST_H

#include "DXATest.h"




class ForearmTest : public DXATest
{
public:
    explicit ForearmTest(QObject *parent = nullptr);
    QList<QString> testKeys {
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
    };
};

#endif // FOREARMTEST_H
