#ifndef GRIP_STRENGTH_MEASUREMENT_H
#define GRIP_STRENGTH_MEASUREMENT_H

#include "../../measurement.h"

/*!
* \class GripStrengthMeasurement
* \brief A GripStrengthMeasurement class
*
* \sa Measurement
*/

typedef QMap<QString, QString> q_stringMap;

class GripStrengthMeasurement : public Measurement
{
public:
    GripStrengthMeasurement() = default;
    ~GripStrengthMeasurement() = default;

    void fromRecord(const QJsonObject* record);

    bool isValid() const override;
    void simulate(int i);

    QString toString() const override;


    static const q_stringMap trialMap;
};

Q_DECLARE_METATYPE(GripStrengthMeasurement);

QDebug operator<<(QDebug dbg, const GripStrengthMeasurement&);

#endif // GRIP_STRENGTH_MEASUREMENT_H


