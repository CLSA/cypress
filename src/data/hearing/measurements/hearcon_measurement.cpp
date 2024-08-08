#include "hearcon_measurement.h"

HearconMeasurement::HearconMeasurement() {}

bool HearconMeasurement::isValid() const
{
    const bool hasRequiredFields = hasAttribute("side") && hasAttribute("test");
    const bool hasResult = hasAttribute("level");

    return hasRequiredFields && hasResult;
}
