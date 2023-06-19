#ifndef TONOMETER_MEASUREMENT_H
#define TONOMETER_MEASUREMENT_H

#include "../../Measurement.h"

/*!
* \class TonometerMeasurement
* \brief A Tonometer Measurement class
*
* Tonometer measurements are derived from MS Access database files.
* This class facilitates converting the given inputs to output.
*
* \sa Measurement, TonometerTest
*/

typedef QMap<QString,QString> q_stringMap;

class TonometerMeasurement : public Measurement
{
public:
    TonometerMeasurement() = default;
    ~TonometerMeasurement() = default;

    void fromVariant(const QVariantMap&);

    bool isValid() const override;

    QString toString() const override;

    void simulate(const QString&);

    static const q_stringMap variableLUT;
    static const q_stringMap unitsLUT;
    static q_stringMap initVariableLUT();
    static q_stringMap initUnitsLUT();
};

Q_DECLARE_METATYPE(TonometerMeasurement);

QDebug operator<<(QDebug dbg, const TonometerMeasurement&);

#endif // TONOMETER_MEASUREMENT_H
