#include "forearm_measurement.h"
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
    Q_UNUSED(no_keys)
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
