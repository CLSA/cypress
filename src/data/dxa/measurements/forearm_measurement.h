#ifndef FOREARM_MEASUREMENT_H
#define FOREARM_MEASUREMENT_H

#include "dxa_measurement.h"

class ForearmMeasurement : public DXAMeasurement
{
public:
    ForearmMeasurement();

    // String representation for debug and GUI display purposes
    //
    virtual QString toString() const;

    virtual QStringList toStringList(const bool& no_keys = false) const;

    virtual bool isValid() const;

    // String keys are converted to snake_case
    //
    virtual QJsonObject toJsonObject() const;
};

#endif // FOREARM_MEASUREMENT_H
