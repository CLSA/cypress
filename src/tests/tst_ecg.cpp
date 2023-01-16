#include <QtTest>

#include "tst_ecg.h"
#include "../data/ecg/tests/ECGTest.h"

TestEcg::TestEcg()
{
    qDebug("constructor");
}

TestEcg::~TestEcg()
{

    qDebug("destructor");
}

void TestEcg::initTestCase()
{
}

void TestEcg::cleanupTestCase()
{
}

void TestEcg::testCase1()
{
    ECGTest test;
    QVERIFY(test.isValid());
}

void TestEcg::testCase2()
{
    qDebug("test case 1");
}


