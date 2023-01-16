#include "tst_audiometer.h"
//#include "../data/grip_strength/tests/AudiometerTest.h"

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
    qDebug() << "initTestCase";
}

void TestAudiometer::cleanupTestCase()
{
    qDebug() << "cleanUpTestCase";
}

void TestAudiometer::testCase1()
{
    qDebug() << "testCase1";
    //AudiometerTest test;
    //QVERIFY(test.isValid());
}

