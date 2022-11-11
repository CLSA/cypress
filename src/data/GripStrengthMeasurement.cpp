#include "GripStrengthMeasurement.h"

#include <QDebug>
#include <QJsonObject>
#include "../auxiliary/Utilities.h"

const q_stringMap GripStrengthMeasurement::trialMap =
{
    {"ExamID", "exam_id"},
    {"TestID", "test_id"},
    {"Position", "position"},
    {"Side","trial_side"},
    {"Maximum","trial_max"},
    {"Average","exam_average"},
    {"CV","exam_cv"},

    {"Rep1", "rep1"},
    {"Rep2", "rep2"},
    {"Rep3", "rep3"},
    {"Rep4", "rep4"},
    {"Rep5", "rep5"},
    {"Rep6", "rep6"},
    {"Rep7", "rep7"},
    {"Rep8", "rep8"},
    {"Rep9", "rep9"},
    {"Rep10", "rep10"},
    {"Rep1Exclude", "rep1Exclude"},
    {"Rep2Exclude", "rep2Exclude"},
    {"Rep3Exclude", "rep3Exclude"},
    {"Rep4Exclude", "rep4Exclude"},
    {"Rep5Exclude", "rep5Exclude"},
    {"Rep6Exclude", "rep6Exclude"},
    {"Rep7Exclude", "rep7Exclude"},
    {"Rep8Exclude", "rep8Exclude"},
    {"Rep9Exclude", "rep9Exclude"},
    {"Rep10Exclude", "rep10Exclude"},
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
    bool okResult = true;
    foreach(const auto key, trialMap.values())
    {
        if (!hasAttribute(key))
        {
            qDebug() << "trial measurement missing trial attribute" << key;
            okResult = false;
            break;
        }
    }
    return okResult;
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

GripStrengthMeasurement GripStrengthMeasurement::simulate()
{
    GripStrengthMeasurement simulatedMeasurement;
    return simulatedMeasurement;
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
