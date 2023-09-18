#ifndef RETINALCAMERAMEASUREMENT_H
#define RETINALCAMERAMEASUREMENT_H

#include "data/Measurement.h"

class RetinalCameraMeasurement : public Measurement
{
public:
    RetinalCameraMeasurement();

    // Measurement interface
public:
    bool isValid() const;
};

#endif // RETINALCAMERAMEASUREMENT_H
