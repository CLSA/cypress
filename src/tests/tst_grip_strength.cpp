#include <QtTest>

//#include "../data/grip_strength/tests/GripStrengthTest.h"

#include "tst_grip_strength.h"

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
    //GripStrengthTest test;
    //test.readGripTestOptions();
    //test.readGripTestResults();
    //QVERIFY(test.isValid());

}

void TestGripStrength::testCase2()
{
    qDebug("test case 1");
}


