#ifndef WEIGH_SCALE_TEST_H
#define WEIGH_SCALE_TEST_H

#include "../../test_base.h"

/*!
 * \class WeighScaleTest
 * \brief A Rice Lake digital weigh scale test class
 *
 * Concrete implementation of TestBase using WeightMeasurement
 * class specialization.  This class parses the binary data stream
 * obtained as a QByteArray from the RS232 interface controlled by the WeighScaleManager
 * class.
 *
 * NOTES: measurements are restricted to being a minimum of DELAY seconds
 * apart.
 *
 * \sa WeighScaleManager, WeightMeasurement, Measurement
 *
 */

class WeighScaleTest : public TestBase
{
public:
    WeighScaleTest();
    ~WeighScaleTest() = default;

    void fromArray(const QByteArray &);

    // String representation for debug and GUI display purposes
    //
    QString toString() const override;

    bool isValid() const override;

    // String keys are converted to snake_case
    //
    QJsonObject toJsonObject() const override;

    void simulate() override;
    double calculateAverage();

    // ms of delay between performing a measurement to allow for
    // step on/off without implementing a delay timer between
    // UI measure button click events
    //
    static const qint64 DELAY { 5 };

private:
    QList<QString> m_outputKeyList;

};

Q_DECLARE_METATYPE(WeighScaleTest);

#endif // WEIGH_SCALE_TEST_H
