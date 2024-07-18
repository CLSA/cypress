#include "hearcon_measurement.h"

HearconMeasurement::HearconMeasurement() {}

QString HearconMeasurement::toString() const
{
    return "";
}

bool HearconMeasurement::isValid() const
{
    return false;
}

QJsonObject HearconMeasurement::toJsonObject() const
{
    QJsonObject json {};
    return json;
}
