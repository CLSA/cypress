#include <QtTest>

//#include "../data/ultrasound/tests/ultrasound.h"

class TestUltrasound: public QObject
{
    Q_OBJECT

public:
    TestUltrasound();
    ~TestUltrasound();

private:
    QJsonObject expectedInput;
    QJsonObject expectedOutput;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase2();
};

TestUltrasound::TestUltrasound()
{
    qDebug("constructor");
}

TestUltrasound::~TestUltrasound()
{

    qDebug("destructor");
}

void TestUltrasound::initTestCase()
{
}

void TestUltrasound::cleanupTestCase()
{
}

void TestUltrasound::testCase1()
{
    //GripStrengthTest test;
    //test.readGripTestOptions();
    //test.readGripTestResults();
    //QVERIFY(test.isValid());

}

void TestUltrasound::testCase2()
{
    qDebug("test case 1");
}

QTEST_APPLESS_MAIN(TestUltrasound)

#include "tst_ultrasound.moc"
