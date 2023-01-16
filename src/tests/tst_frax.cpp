#include <QtTest>

#include "tst_frax.h"
#include "../data/frax/tests/FraxTest.h"


TestFrax::TestFrax()
{
    qDebug("constructor");
}

TestFrax::~TestFrax()
{

    qDebug("destructor");
}

void TestFrax::initTestCase()
{
}

void TestFrax::cleanupTestCase()
{
}

void TestFrax::testCase1()
{
    //FraxTest test;
    //QVERIFY(test.isValid());
}

void TestFrax::testCase2()
{
    qDebug("test case 1");
}


