#include "whole_body_measurement.h"
#include <QJsonObject>

// { "WBTOT_BMD",      "NULL" },

WholeBodyScanMeasurement::WholeBodyScanMeasurement()
{

}

QString WholeBodyScanMeasurement::toString() const
{
    return "";
};

QStringList WholeBodyScanMeasurement::toStringList(const bool& no_keys) const
{
    Q_UNUSED(no_keys)
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
