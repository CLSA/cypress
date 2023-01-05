#include <QtTest>

#include "../data/weigh_scale/tests/WeighScaleTest.h"

class TestWeighScale: public QObject
{
    Q_OBJECT

public:
    TestWeighScale();
    ~TestWeighScale();

private:
    QJsonObject expectedInput;
    QJsonObject expectedOutput;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase2();
};

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
    WeighScaleTest test;
    QVERIFY(test.isValid());
}

void TestWeighScale::testCase2()
{
    qDebug("test case 1");
}

QTEST_APPLESS_MAIN(TestWeighScale)

#include "tst_weigh_scale.moc"
