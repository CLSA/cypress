#ifndef CIMTVIVIDIMEASUREMENT_H
#define CIMTVIVIDIMEASUREMENT_H

#include "Measurement.h"

#include <QJsonObject>

class CimtVividIMeasurement : public Measurement
{
public:
    CimtVividIMeasurement();
    // Measurement interface
public:
    QString toString() const override;
    QStringList toStringList(const bool &no_keys) const override;
    bool isValid() const override;
    QJsonObject toJsonObject() const override;
};

#endif // CIMTVIVIDIMEASUREMENT_H
