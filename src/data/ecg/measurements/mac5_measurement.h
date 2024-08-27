#ifndef MAC5_MEASUREMENT_H
#define MAC5_MEASUREMENT_H

#include "data/measurement.h"

class Mac5Measurement : public Measurement
{
public:
    Mac5Measurement();

    // Measurement interface
public:
    QString toString() const;
    bool isValid() const;
    QJsonObject toJsonObject() const;
};

#endif // MAC5_MEASUREMENT_H
