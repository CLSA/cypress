#include <QtTest>

#include "../data/tonometer/tests/TonometerTest.h"

class TestTonometer: public QObject
{
    Q_OBJECT

public:
    TestTonometer();
    ~TestTonometer();

private:
    QJsonObject expectedInput;
    QJsonObject expectedOutput;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase2();
};

TestTonometer::TestTonometer()
{
    qDebug("constructor");
}

TestTonometer::~TestTonometer()
{

    qDebug("destructor");
}

void TestTonometer::initTestCase()
{
}

void TestTonometer::cleanupTestCase()
{
}

void TestTonometer::testCase1()
{
    TonometerTest test;
    QVERIFY(test.isValid());
}

void TestTonometer::testCase2()
{
    qDebug("test case 1");
}

QTEST_APPLESS_MAIN(TestTonometer)

#include "tst_tonometer.moc"
