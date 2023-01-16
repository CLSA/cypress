#ifndef TST_BLOOD_PRESSURE_H
#define TST_BLOOD_PRESSURE_H

#include <QTest>
#include <QJsonObject>

class TestBloodPressure: public QObject
{
    Q_OBJECT

public:
    TestBloodPressure();
    ~TestBloodPressure();

private:
    QJsonObject expectedInput;
    QJsonObject expectedOutput;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
};

#endif // TST_BLOOD_PRESSURE_H
