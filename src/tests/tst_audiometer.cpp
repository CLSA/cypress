#include <QtTest>

//#include "../data/grip_strength/tests/AudiometerTest.h"

class TestAudiometer: public QObject
{
    Q_OBJECT

public:
    TestAudiometer();
    ~TestAudiometer();

private:
    QJsonObject expectedInput;
    QJsonObject expectedOutput;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
};

TestAudiometer::TestAudiometer()
{
    qDebug("constructor");
}

TestAudiometer::~TestAudiometer()
{

    qDebug("destructor");
}

void TestAudiometer::initTestCase()
{
}

void TestAudiometer::cleanupTestCase()
{
}

void TestAudiometer::testCase1()
{
    //AudiometerTest test;
    //QVERIFY(test.isValid());
}

QTEST_APPLESS_MAIN(TestAudiometer)

#include "tst_audiometer.moc"
