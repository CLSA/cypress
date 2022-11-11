#include "DXAMeasurement.h"
#include <QJsonObject>

DXAMeasurement::DXAMeasurement()
{

}

QString DXAMeasurement::toString() const
{
   return "";
};

QStringList DXAMeasurement::toStringList(const bool& no_keys) const
{
    return QStringList {{}};
};

bool DXAMeasurement::isValid() const
{
   return false;
};

// String keys are converted to snake_case
//
QJsonObject DXAMeasurement::toJsonObject() const
{
   return QJsonObject();
};
