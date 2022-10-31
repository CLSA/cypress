#include "WholeBodyScanMeasurement.h"
#include <QJsonObject>

WholeBodyScanMeasurement::WholeBodyScanMeasurement()
{

}

QString WholeBodyScanMeasurement::toString() const
{
    return "";
};

QStringList WholeBodyScanMeasurement::toStringList(const bool& no_keys) const
{
    return QStringList {{}};
};

bool WholeBodyScanMeasurement::isValid() const
{
    return false;
};

QJsonObject WholeBodyScanMeasurement::toJsonObject() const
{
    return QJsonObject();
};
