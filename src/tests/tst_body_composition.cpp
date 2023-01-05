#include <QtTest>

#include "../data/body_composition/tests/BodyCompositionTest.h"

class TestBodyComposition: public QObject
{
    Q_OBJECT

public:
    TestBodyComposition();
    ~TestBodyComposition();

private:
    QJsonObject expectedInput;
    QJsonObject expectedOutput;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
};

TestBodyComposition::TestBodyComposition()
{
    qDebug("constructor");
}

TestBodyComposition::~TestBodyComposition()
{

    qDebug("destructor");
}

void TestBodyComposition::initTestCase()
{
}

void TestBodyComposition::cleanupTestCase()
{
}

void TestBodyComposition::testCase1()
{
    BodyCompositionTest test;
    QVERIFY(test.isValid());
}

QTEST_APPLESS_MAIN(TestBodyComposition)

#include "tst_body_composition.moc"
