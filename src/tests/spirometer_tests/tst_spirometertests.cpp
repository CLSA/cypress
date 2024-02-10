#include <QtTest>
#include <QDebug>

#include "data/spirometer/tests/spirometer_test.h"
// add necessary includes here

class SpirometerTests : public QObject
{
    Q_OBJECT

public:
    SpirometerTests();
    ~SpirometerTests();

private slots:
    void test_case1();
};

SpirometerTests::SpirometerTests() {

}

SpirometerTests::~SpirometerTests() {}

void SpirometerTests::test_case1() {
    SpirometerTest test;

    test.fromFile("C:/work/clsa/Test Data/spirometer/OnyxOut.xml");

    qDebug() << "spirometer valid: " << test.isValid();
    qDebug() << test.toJsonObject();
}

QTEST_APPLESS_MAIN(SpirometerTests)

#include "tst_spirometertests.moc"
