#ifndef WEIGHT_MEASUREMENT_H
#define WEIGHT_MEASUREMENT_H

#include "../../measurement.h"

/*!
 * \class WeightMeasurement
 * \brief A WeightMeasurement class
 *
 * Measurements of weight are derived from a
 * Rice Lake digital weigh scale over RS232 serial
 * communication.  This class facilitates parsing
 * QByteArray input from the QSerialPort of the WeighScaleManager
 * class into measurement characteristics, such as value, units etc.
 *
 * \sa Measurement
 */

class WeightMeasurement : public Measurement
{
public:
    WeightMeasurement();
    ~WeightMeasurement() = default;

    WeightMeasurement(const double weight, const QString unit);

    void fromArray(const QByteArray &);

    // String representation for debug and GUI display purposes
    //
    QString toString() const override;

    bool isValid() const override;

    bool isZero() const;
    void simulate();
};

Q_DECLARE_METATYPE(WeightMeasurement);

QDebug operator<<(QDebug dbg, const WeightMeasurement &);

#endif // WEIGHT_MEASUREMENT_H
