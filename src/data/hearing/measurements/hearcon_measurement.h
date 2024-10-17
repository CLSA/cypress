#ifndef HEARCON_MEASUREMENT_H
#define HEARCON_MEASUREMENT_H

#include "data/measurement.h"

#include <QJsonObject>

class HearconMeasurement : public Measurement
{
public:
    HearconMeasurement();

    HearconMeasurement(const QString &side, const QString &test, const int level, const bool pass);

    // Measurement interface
public:
    bool isValid() const override;
};

#endif // HEARCON_MEASUREMENT_H
