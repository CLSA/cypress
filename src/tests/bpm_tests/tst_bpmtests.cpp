#include <QtTest>
#include <QDebug>

#include "managers/blood_pressure/bpm_messages.h"

class BpmTests : public QObject
{
    Q_OBJECT

public:
    BpmTests();
    ~BpmTests();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();

};

BpmTests::BpmTests()
{

}

BpmTests::~BpmTests()
{

}

void BpmTests::initTestCase()
{
    qDebug() << "init test case";

}

void BpmTests::cleanupTestCase()
{
    qDebug() << "cleanup test case";
}

void BpmTests::test_case1()
{
    qDebug() << "test case 1";
}

QTEST_APPLESS_MAIN(BpmTests)

#include "tst_bpmtests.moc"
