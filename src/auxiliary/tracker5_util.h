#ifndef TRACKER5_UTIL_H
#define TRACKER5_UTIL_H


class Tracker5Util
{
public:
    static double asNewtons(int thousanthsPounds);
    static double asKg(int thousanthsPounds);

    constexpr static double ONE_POUND_FORCE_IN_NEWTONS = 4.4482216152605;
    constexpr static double ONE_POUND_FORCE_IN_KILOGRAMS = 4.4482216152605 / 9.80665;
};

#endif // TRACKER5_UTIL_H
