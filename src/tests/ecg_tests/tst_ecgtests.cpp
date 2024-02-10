#include <QtTest>

#include "data/ecg/tests/ecg_test.h"

// add necessary includes here

class ECGTests : public QObject
{
    Q_OBJECT

public:
    ECGTests();
    ~ECGTests();

private slots:
    void test_case1();
};

ECGTests::ECGTests() {}

ECGTests::~ECGTests() {}

void ECGTests::test_case1() {
    ECGTest test;

    test.fromFile("C:/work/clsa/Test Data/ecg/Ecg.xml");

    qDebug() << "VALID: " << test.isValid();

    qDebug() << test.toJsonObject();

}

QTEST_APPLESS_MAIN(ECGTests)

#include "tst_ecgtests.moc"
