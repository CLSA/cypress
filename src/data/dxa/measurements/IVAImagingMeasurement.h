#ifndef IVAIMAGINGMEASUREMENT_H
#define IVAIMAGINGMEASUREMENT_H

#include "data/Measurement.h"

class IVAImagingMeasurement : public Measurement
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

#endif // IVAIMAGINGMEASUREMENT_H
