#ifndef RETINALCAMERAMEASUREMENT_H
#define RETINALCAMERAMEASUREMENT_H

#include "data/measurement.h"

class RetinalCameraMeasurement : public Measurement
{
public:
    RetinalCameraMeasurement();

public:
    bool isValid() const override;

public:
    QString toString() const override;
};

#endif // RETINALCAMERAMEASUREMENT_H
