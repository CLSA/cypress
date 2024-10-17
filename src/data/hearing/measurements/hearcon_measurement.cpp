#include "hearcon_measurement.h"

HearconMeasurement::HearconMeasurement() {}

HearconMeasurement::HearconMeasurement(const QString &side, const QString &test, const int level, const bool pass)
{
    setAttribute("side", side.toLower());
    setAttribute("test", test);
    setAttribute("level", level, "dB");
    setAttribute("pass", pass);
}

bool HearconMeasurement::isValid() const
{
    const bool hasRequiredFields = hasAttribute("side") && hasAttribute("test");
    const bool hasResult = hasAttribute("level");

    return hasRequiredFields && hasResult;
}
