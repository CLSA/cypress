#include <QtTest>

#include "tst_weigh_scale.h"
#include "../data/weigh_scale/tests/WeighScaleTest.h"

TestWeighScale::TestWeighScale()
{
    qDebug("constructor");
}

TestWeighScale::~TestWeighScale()
{

    qDebug("destructor");
}

void TestWeighScale::initTestCase()
{
}

void TestWeighScale::cleanupTestCase()
{
}

void TestWeighScale::testCase1()
{
    //WeighScaleTest test;
    //QVERIFY(test.isValid());
}

void TestWeighScale::testCase2()
{
    qDebug("test case 1");
}


