#ifndef WEIGHTMEASUREMENT_H
#define WEIGHTMEASUREMENT_H

#include "MeasurementBase.h"

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
 * \sa MeasurementBase
 */

class WeightMeasurement :  public MeasurementBase
{   
public:
    void fromArray(const QByteArray &);

    QString toString() const;

    bool isValid() const;

    bool isZero() const;
};

Q_DECLARE_METATYPE(WeightMeasurement);

#endif // WEIGHTMEASUREMENT_H