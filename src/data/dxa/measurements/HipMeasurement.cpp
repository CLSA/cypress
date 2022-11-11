#include "HipMeasurement.h"
#include <QJsonObject>


HipMeasurement::HipMeasurement()
{

}

QString HipMeasurement::toString() const
{
    return "";
};

QStringList HipMeasurement::toStringList(const bool& no_keys) const
{
    return QStringList {{}};
};

bool HipMeasurement::isValid() const
{
    return false;
};

QJsonObject HipMeasurement::toJsonObject() const
{
    return QJsonObject();
};
