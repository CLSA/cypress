#include <QtTest>

#include "../data/ecg/tests/ECGTest.h"

class TestGripStrength: public QObject
{
    Q_OBJECT

public:
    TestGripStrength();
    ~TestGripStrength();

private:
    QJsonObject expectedInput;
    QJsonObject expectedOutput;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase2();
};

TestGripStrength::TestGripStrength()
{
    qDebug("constructor");
}

TestGripStrength::~TestGripStrength()
{

    qDebug("destructor");
}

void TestGripStrength::initTestCase()
{
}

void TestGripStrength::cleanupTestCase()
{
}

void TestGripStrength::testCase1()
{
    ECGTest test;
    QVERIFY(test.isValid());
}

void TestGripStrength::testCase2()
{
    qDebug("test case 1");
}

QTEST_APPLESS_MAIN(TestGripStrength)

#include "tst_ecg.moc"
