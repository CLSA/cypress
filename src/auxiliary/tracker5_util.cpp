#include "tracker5_util.h"

double Tracker5Util::asNewtons(int thousanthsPounds) {
    return ((double) thousanthsPounds) * ONE_POUND_FORCE_IN_NEWTONS / 1000.0;
}

double Tracker5Util::asKg(int thousanthsPounds) {
    return ((double) thousanthsPounds) * ONE_POUND_FORCE_IN_KILOGRAMS / 1000.0;
}
