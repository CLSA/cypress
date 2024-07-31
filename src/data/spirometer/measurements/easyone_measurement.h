#ifndef EASYONE_MEASUREMENT_H
#define EASYONE_MEASUREMENT_H

#include "data/measurement.h"

#include <QJsonObject>

class EasyoneMeasurement : public Measurement
{
public:
    EasyoneMeasurement();

    // Measurement interface
public:
    QString toString() const;
    bool isValid() const;
    QJsonObject toJsonObject() const;
};

#endif // EASYONE_MEASUREMENT_H
