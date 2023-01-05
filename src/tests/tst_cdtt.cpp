#include <QtTest>

#include "../data/cdtt/tests/CDTTTest.h"

class TestCDTT: public QObject
{
    Q_OBJECT

public:
    TestCDTT();
    ~TestCDTT();

private:
    QJsonObject expectedInput;
    QJsonObject expectedOutput;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase2();
};

TestCDTT::TestCDTT()
{
    qDebug("constructor");
}

TestCDTT::~TestCDTT()
{

    qDebug("destructor");
}

void TestCDTT::initTestCase()
{
}

void TestCDTT::cleanupTestCase()
{
}

void TestCDTT::testCase1()
{
    CDTTTest test;
    QVERIFY(test.isValid());

}

void TestCDTT::testCase2()
{
    qDebug("test case 1");
}

QTEST_APPLESS_MAIN(TestCDTT)

#include "tst_cdtt.moc"
