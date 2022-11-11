#ifndef FOREARMMEASUREMENT_H
#define FOREARMMEASUREMENT_H

#include "data/Measurement.h"

class ForearmMeasurement : public Measurement
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

#endif // FOREARMMEASUREMENT_H
