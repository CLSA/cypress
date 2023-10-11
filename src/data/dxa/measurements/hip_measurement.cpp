#include "hip_measurement.h"
#include <QJsonObject>

// hip
// { "NECK_BMD",       "1..." },
// { "TROCH_BMD",      ".2.." },
// { "INTER_BMD",      "..3." },
// { "WARDS_BMD",      "...4" },
// { "HTOT_BMD",       "123." },

HipMeasurement::HipMeasurement()
{

}

QString HipMeasurement::toString() const
{
    return "";
};

QStringList HipMeasurement::toStringList(const bool& no_keys) const
{
    Q_UNUSED(no_keys)
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
