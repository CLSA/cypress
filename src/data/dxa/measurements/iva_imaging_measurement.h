#ifndef IVA_IMAGING_MEASUREMENT_H
#define IVA_IMAGING_MEASUREMENT_H

#include "dxa_measurement.h"

class IVAImagingMeasurement : public DXAMeasurement
{
public:
    IVAImagingMeasurement();

    // String representation for debug and GUI display purposes
    //
    virtual QString toString() const;

    virtual QStringList toStringList(const bool& no_keys = false) const;

    virtual bool isValid() const;

    // String keys are converted to snake_case
    //
    virtual QJsonObject toJsonObject() const;
};

#endif // IVA_IMAGING_MEASUREMENT_H
