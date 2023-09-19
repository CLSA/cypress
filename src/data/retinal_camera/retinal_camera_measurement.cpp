#include "retinal_camera_measurement.h"

#include <QJsonObject>

RetinalCameraMeasurement::RetinalCameraMeasurement()
{
    setAttribute("EYE_PICT_VENDOR", QVariant());
    setAttribute("EYE_SIDE_VENDOR", QVariant());
}

bool RetinalCameraMeasurement::isValid() const
{
    return !getAttribute("EYE_PICT_VENDOR").isNull() && \
           !getAttribute("EYE_SIDE_VENDOR").isNull();
}

QString RetinalCameraMeasurement::toString() const
{
    return getAttribute("EYE_SIDE_VENDOR").toString() + getAttribute("EYE_PICT_VENDOR").toString();
}

QStringList RetinalCameraMeasurement::toStringList(const bool &no_keys) const
{
    return QStringList { };
}
