#ifndef APSPINEMEASUREMENT_H
#define APSPINEMEASUREMENT_H

#include "DXAMeasurement.h"

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

#endif // APSPINEMEASUREMENT_H
