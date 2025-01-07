#include "grip_strength_measurement.h"
#include "auxiliary/Utilities.h"

#include <QDebug>
#include <QJsonObject>
#include <QRandomGenerator>

const q_stringMap GripStrengthMeasurement::trialMap =
{
    {"Position", "position"},
    {"Side","side"},

    {"Rep1", "rep1"},
    {"Rep2", "rep2"},
    {"Rep3", "rep3"},
};


void GripStrengthMeasurement::fromRecord(const QJsonObject* record)
{
    foreach(const auto tag, trialMap.toStdMap()) {
        if (record->contains(tag.first)) {
            setAttribute(tag.second, record->value(tag.first).toVariant());
        }
    }
}

bool GripStrengthMeasurement::isValid() const
{
    foreach(const auto key, trialMap.values())
    {
        if (!hasAttribute(key))
        {
            qDebug() << "Does not have attribute" << key;
            return false;
        }
    }

    return true;
}

QString GripStrengthMeasurement::toString() const
{
    return QString("Side: %1 Rung: %2 Average: %3 Max: %4 CV: %5")
        .arg(getAttribute("trial_side").value().toString())
        .arg(getAttribute("trial_rung_position").value().toString())
        .arg(getAttribute("exam_average").value().toString())
        .arg(getAttribute("trial_max").value().toString())
        .arg(getAttribute("exam_cv").value().toString());
}

QDebug operator<<(QDebug dbg, const GripStrengthMeasurement& item)
{
    const QString measurementStr = item.toString();
    if (measurementStr.isEmpty())
        dbg.nospace() << "GripStrength Measurement()";
    else
        dbg.nospace() << "GripStrength Measurement(" << measurementStr << " ...)";
    return dbg.maybeSpace();
}
