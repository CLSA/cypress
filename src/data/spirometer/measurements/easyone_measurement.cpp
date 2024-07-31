#include "easyone_measurement.h"

EasyoneMeasurement::EasyoneMeasurement() {}

QString EasyoneMeasurement::toString() const
{
    return "EasyoneMeasurement";
}

bool EasyoneMeasurement::isValid() const
{
    return true;
}

QJsonObject EasyoneMeasurement::toJsonObject() const
{
    QJsonObject object {};
    return object;
}
