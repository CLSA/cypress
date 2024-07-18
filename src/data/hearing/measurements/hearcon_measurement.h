#ifndef HEARCON_MEASUREMENT_H
#define HEARCON_MEASUREMENT_H

#include "data/measurement.h"

#include <QJsonObject>

class HearconMeasurement : public Measurement
{
public:
    HearconMeasurement();

    // Measurement interface
public:
    bool isValid() const override;
    QString toString() const override;
    QJsonObject toJsonObject() const override;
};

#endif // HEARCON_MEASUREMENT_H
