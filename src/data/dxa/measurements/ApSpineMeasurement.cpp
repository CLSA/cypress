#include "ApSpineMeasurement.h"
#include <QJsonObject>
#include <QStringList>

ApSpineMeasurement::ApSpineMeasurement()
{

}

QString ApSpineMeasurement::toString() const
{
   return "";
};

QStringList ApSpineMeasurement::toStringList(const bool& no_keys) const
{
    return QStringList {{}};
};

bool ApSpineMeasurement::isValid() const
{
   return false;
};

// String keys are converted to snake_case
//
QJsonObject ApSpineMeasurement::toJsonObject() const
{
   return QJsonObject();
};
