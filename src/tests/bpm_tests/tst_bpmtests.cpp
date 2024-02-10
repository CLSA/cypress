#include <QtTest>
#include <QDebug>

#include "data/blood_pressure/tests/blood_pressure_test.h"

class BpmTests : public QObject
{
    Q_OBJECT

public:
    BpmTests();
    ~BpmTests();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();

};

BpmTests::BpmTests()
{

}

BpmTests::~BpmTests()
{

}

void BpmTests::initTestCase()
{
    qDebug() << "init test case";

}

void BpmTests::cleanupTestCase()
{
    qDebug() << "cleanup test case";
}

void BpmTests::test_bpm()
{
    BloodPressureTest test;

    test.simulate();

    qDebug() << test.toJsonObject();
}

QTEST_APPLESS_MAIN(BpmTests)

#include "tst_bpmtests.moc"
