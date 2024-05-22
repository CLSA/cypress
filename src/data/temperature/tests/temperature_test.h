#ifndef TEMPERATURE_TEST_H
#define TEMPERATURE_TEST_H

#include "data/test_base.h"

/*!
 * \class TemperatureTest
 * \brief A Masimo TIR-1 bluetooth thermometer test class
 *
 * Concrete implementation of TestBase using TemperatureMeasurement
 * class specialization.
 *
 * \sa BluetoothLEManager, TemperatureMeasurement, Measurement
 *
 */

class TemperatureTest : public TestBase {
public:
    TemperatureTest() = default;
    ~TemperatureTest() = default;

    void fromArray( const QByteArray &);

    QString toString() const override;

    bool isValid() const override;

    QJsonObject toJsonObject() const override;

};

Q_DECLARE_METATYPE(TemperatureTest);

#endif // TEMPERATURE_TEST_H
