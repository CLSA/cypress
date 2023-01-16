#include <QtTest>

#include "tst_tonometer.h"
#include "../data/tonometer/tests/TonometerTest.h"

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
    //TonometerTest test;
    //QVERIFY(test.isValid());
}

void TestTonometer::testCase2()
{
    qDebug("test case 1");
}


