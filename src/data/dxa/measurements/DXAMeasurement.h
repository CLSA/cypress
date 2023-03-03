#ifndef DXAMEASUREMENT_H
#define DXAMEASUREMENT_H

#include "../../Measurement.h"

class DXAMeasurement : public Measurement
{
public:
    DXAMeasurement();

    // String representation for debug and GUI display purposes
    //
    virtual QString toString() const;

    virtual QStringList toStringList(const bool& no_keys = false) const;

    virtual bool isValid() const;

    // String keys are converted to snake_case
    //
    virtual QJsonObject toJsonObject() const;
};

#endif // DXAMEASUREMENT_H
