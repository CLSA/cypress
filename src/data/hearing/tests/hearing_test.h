#ifndef HEARING_TEST_H
#define HEARING_TEST_H

#include "../../test_base.h"
#include "../measurements/hearing_measurement.h"

/*!
 * \class HearingTest
 * \brief A Trementrics RA300 Audiometer test class
 *
 * Concrete implementation of TestBase using HearingMeasurement
 * class specialization.  This class parses the binary data stream
 * obtained as a QByteArray from the RS232 interface controlled by the AudiometerManager
 * class.  Test parameters such as participant ID (barcode) are stored
 * as meta data (see Measurement) while hearing threshold levels (HTL)
 * are stored 8 frequency tests per side left and right for a total of
 * 16 tests.  In case of a HTL error code received in the byte array,
 * the measurement data for the frequency in question consists of an
 * error code string and a resolution outcome string.
 *
 * \sa AudiometerManager, HearingMeasurement, Measurement
 *
 */

class HearingTest : public TestBase
{
public:
    HearingTest();
    ~HearingTest() = default;

    void fromArray(const QByteArray &);

    // String representation for debug and GUI display purposes
    //
    QString toString() const override;

    bool isValid() const override;

    bool isPartial() const;

    void simulate() override;

    const HearingMeasurement &getMeasurement(const QString &, const int &) const;

    // String keys are converted to snake_case
    //
    QJsonObject toJsonObject() const override;

private:
    QList<QString> m_outputKeyList;
    QByteArray m_array;
    QString readArray(const quint8 &, const quint8 &) const;

    QString   readPatientID() const;
    QString   readTestID() const;
    QDateTime readTestDateTime() const;
    QDate     readCalibrationDate() const;
    QString   readExaminerID() const;
    QList<HearingMeasurement> readHearingThresholdLevels(const QString& side) const;
};

Q_DECLARE_METATYPE(HearingTest);

#endif // HEARING_TEST_H
