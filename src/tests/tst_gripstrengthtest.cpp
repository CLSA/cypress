#include <QtTest>

#include "../data/grip_strength/tests/GripStrengthTest.h"

class TestGripStrength: public QObject
{
    Q_OBJECT

public:
    TestGripStrength();
    ~TestGripStrength();

private:
    QJsonObject expectedInput;
    QJsonObject expectedOutput;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();


};

TestGripStrength::TestGripStrength()
{

}

TestGripStrength::~TestGripStrength()
{

}

void TestGripStrength::initTestCase()
{

}

void TestGripStrength::cleanupTestCase()
{

}

void TestGripStrength::test_case1()
{

}

QTEST_APPLESS_MAIN(TestGripStrength)

#include "tst_gripstrengthtest.moc"
