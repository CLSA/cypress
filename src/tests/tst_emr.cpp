#include <QtTest>

//#include "../data/emr/tests/EmrTest.h"

class TestEmr: public QObject
{
    Q_OBJECT

public:
    TestEmr();
    ~TestEmr();

private:
    QJsonObject expectedInput;
    QJsonObject expectedOutput;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase2();
};

TestEmr::TestEmr()
{
    qDebug("constructor");
}

TestEmr::~TestEmr()
{

    qDebug("destructor");
}

void TestEmr::initTestCase()
{
}

void TestEmr::cleanupTestCase()
{
}

void TestEmr::testCase1()
{
    //GripStrengthTest test;
    //test.readGripTestOptions();
    //test.readGripTestResults();
    //QVERIFY(test.isValid());
}

void TestEmr::testCase2()
{
    qDebug("test case 1");
}

QTEST_APPLESS_MAIN(TestEmr)

#include "tst_emr.moc"
