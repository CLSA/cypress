#ifndef BODY_COMPOSITION_MEASUREMENT_H
#define BODY_COMPOSITION_MEASUREMENT_H

#include "../../measurement.h"

class BodyCompositionMeasurement : public Measurement
{
public:
    BodyCompositionMeasurement() = default;
    ~BodyCompositionMeasurement() = default;

    bool isValid() const override;

    static QStringList variableList;
    static QStringList initVariableList();
};

Q_DECLARE_METATYPE(BodyCompositionMeasurement);

QDebug operator<<(QDebug dbg, const BodyCompositionMeasurement &);

#endif // BODY_COMPOSITION_MEASUREMENT_H
