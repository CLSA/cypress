#include "grip_strength_measurement.h"
#include "auxiliary/Utilities.h"

#include <QDebug>
#include <QJsonObject>
#include <QRandomGenerator>

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

    {"Rep1Exclude", "rep1Exclude"},
    {"Rep2Exclude", "rep2Exclude"},
    {"Rep3Exclude", "rep3Exclude"},
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

void GripStrengthMeasurement::simulate(int i)
{
    Q_UNUSED(i);

    setAttribute("rung_position",  2);
    setAttribute("side", "Right");

    double rep1 = Utilities::interp(5, 70, QRandomGenerator::global()->generateDouble());
    double rep2 = Utilities::interp(5, 70, QRandomGenerator::global()->generateDouble());
    double rep3 = Utilities::interp(5, 70, QRandomGenerator::global()->generateDouble());

    double average = (rep1 + rep2 + rep3) / 3;
    double maximum = qMax(qMax(rep1, rep2), rep3);

    setAttribute("rep_1", rep1, "kg");
    setAttribute("rep_2", rep2, "kg");
    setAttribute("rep_3", rep3, "kg");

    setAttribute("average", average, "kg");
    setAttribute("maximum", maximum, "kg");
    setAttribute("cv",      5);
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
