#ifndef AP_SPINE_MEASUREMENT_H
#define AP_SPINE_MEASUREMENT_H

#include "dxa_measurement.h"


class ApSpineMeasurement : public DXAMeasurement
{
public:
    ApSpineMeasurement();

    // String representation for debug and GUI display purposes
    //
    virtual QString toString() const;

    virtual QStringList toStringList(const bool& no_keys = false) const;

    virtual bool isValid() const;

    // String keys are converted to snake_case
    //
    virtual QJsonObject toJsonObject() const;
};

#endif // AP_SPINE_MEASUREMENT_H
