#ifndef BLOOD_PRESSURE_TEST_H
#define BLOOD_PRESSURE_TEST_H

#include "../../blood_pressure/measurements/blood_pressure_measurement.h"
#include "../../test_base.h"

class BloodPressureTest : public TestBase
{
public:
    BloodPressureTest();
    ~BloodPressureTest() = default;

    // reset only the measurement data, keeping cuff and arm side meta data
    //
    void reset() override;

    // Add the average provided by the device as meta data
    //
    void addDeviceAverage(const int& sbpAvg, const int& dbpAvg, const int& pulseAvg);

    // String representation for debug and GUI display purposes
    //
    QString toString() const override;

    bool isValid() const override;

    // simulate a complete test of 6 readings and computed averages
    //
    void simulate() override;

    // String keys are converted to snake_case
    //
    QJsonObject toJsonObject() const override;

    // Verify the review data provided by the device which
    // confirms the last added set of averages
    //
    bool verifyDeviceAverage(const int& sbp, const int& dbp, const int& pulse) const;

    void setCuffSize(const QString&);
    void setSide(const QString&);
    bool armInformationSet() const;

private:
    QList<QString> m_outputKeyList;

    void computeTotalAverage(int sbpTotal, int dbpTotal, int pulseTotal);
    bool hasFirstMeasurement() const;
    bool hasAverage() const;
    bool hasTotalAverage() const;
};

Q_DECLARE_METATYPE(BloodPressureTest);

#endif // BLOOD_PRESSURE_TEST_H

