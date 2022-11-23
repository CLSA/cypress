#include <QJsonObject>
#include "../measurements/IVAImagingMeasurement.h"

IVAImagingMeasurement::IVAImagingMeasurement()
{

}

QString IVAImagingMeasurement::toString() const
{
    return "";
};

QStringList IVAImagingMeasurement::toStringList(const bool& no_keys) const
{
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
