#include <QJsonObject>
#include "iva_imaging_measurement.h"

IVAImagingMeasurement::IVAImagingMeasurement()
{

}

QString IVAImagingMeasurement::toString() const
{
    return "";
};

QStringList IVAImagingMeasurement::toStringList(const bool& no_keys) const
{
    Q_UNUSED(no_keys)
    return QStringList {{}};
};

bool IVAImagingMeasurement::isValid() const
{
    return false;
};

QJsonObject IVAImagingMeasurement::toJsonObject() const
{
    return QJsonObject();
};
