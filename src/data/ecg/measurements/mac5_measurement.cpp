#include "mac5_measurement.h"

#include <QJsonObject>

Mac5Measurement::Mac5Measurement() {}

QString Mac5Measurement::toString() const
{
    return "";
}

bool Mac5Measurement::isValid() const
{
    return true;
}

QJsonObject Mac5Measurement::toJsonObject() const
{
    QJsonObject result{};
    return result;
}
