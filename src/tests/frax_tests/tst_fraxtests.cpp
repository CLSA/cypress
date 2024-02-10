#include <QtTest>

#include "data/frax/tests/frax_test.h"

// add necessary includes here

class FraxTests : public QObject
{
    Q_OBJECT

public:
    FraxTests();
    ~FraxTests();

private slots:
    void test_case1();
};

FraxTests::FraxTests() {}

FraxTests::~FraxTests() {}

void FraxTests::test_case1() {
    FraxTest test;

    test.fromFile("C:/work/clsa/Test Data/frax/output.txt");

    qDebug() << "VALID: " << test.isValid();
    qDebug() << test.toJsonObject();
}

QTEST_APPLESS_MAIN(FraxTests)

#include "tst_fraxtests.moc"
