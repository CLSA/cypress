#include <QtTest>

#include "../data/blood_pressure/tests/BloodPressureTest.h"

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

TestBloodPressure::TestBloodPressure()
{
    qDebug("constructor");
}

TestBloodPressure::~TestBloodPressure()
{

    qDebug("destructor");
}

void TestBloodPressure::initTestCase()
{
}

void TestBloodPressure::cleanupTestCase()
{
}

void TestBloodPressure::testCase1()
{
    BloodPressureTest test;
    QVERIFY(test.isValid());
}

QTEST_APPLESS_MAIN(TestBloodPressure)

#include "tst_blood_pressure.moc"
