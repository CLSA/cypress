#ifndef RETINALCAMERAMEASUREMENT_H
#define RETINALCAMERAMEASUREMENT_H

#include "data/Measurement.h"

class RetinalCameraMeasurement : public Measurement
{
public:
    RetinalCameraMeasurement();

public:
    bool isValid() const override;

public:
    QString toString() const override;
    QStringList toStringList(const bool &no_keys) const override;
};

#endif // RETINALCAMERAMEASUREMENT_H
