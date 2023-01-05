#include <QtTest>

#include "../data/frax/tests/FraxTest.h"

class TestFrax: public QObject
{
    Q_OBJECT

public:
    TestFrax();
    ~TestFrax();

private:
    QJsonObject expectedInput;
    QJsonObject expectedOutput;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase2();
};

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
    FraxTest test;
    QVERIFY(test.isValid());

}

void TestFrax::testCase2()
{
    qDebug("test case 1");
}

QTEST_APPLESS_MAIN(TestFrax)

#include "tst_frax.moc"
