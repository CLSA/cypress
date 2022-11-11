#include "ForearmMeasurement.h"
#include <QJsonObject>

ForearmMeasurement::ForearmMeasurement()
{

}

QString ForearmMeasurement::toString() const
{
    return "";
};

QStringList ForearmMeasurement::toStringList(const bool& no_keys) const
{
    return QStringList {{}};
};

bool ForearmMeasurement::isValid() const
{
    return false;
};

QJsonObject ForearmMeasurement::toJsonObject() const
{
    return QJsonObject();
};
