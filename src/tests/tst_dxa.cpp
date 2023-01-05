#include <QtTest>

//#include "../data/dxa/tests/ApSpineTest.h"
//#include "../data/dxa/tests/ForearmTest.h"
//#include "../data/dxa/tests/HipTest.h"
//#include "../data/dxa/tests/IVAImagingTest.h"
//#include "../data/dxa/tests/WholeBodyScanTest.h"

class TestDxa: public QObject
{
    Q_OBJECT

public:
    TestDxa();
    ~TestDxa();

private:
    QJsonObject expectedInput;
    QJsonObject expectedOutput;

private slots:
    void initTestCase();
    void cleanupTestCase();

    void apSpineTest();
    void forearmTest();
    void hipTest();
    void ivaImagingTest();
    void wholeBodyScanTest();
};

TestDxa::TestDxa()
{
    qDebug("constructor");
}

TestDxa::~TestDxa()
{

    qDebug("destructor");
}

void TestDxa::initTestCase()
{
}

void TestDxa::cleanupTestCase()
{
}

void TestDxa::apSpineTest()
{
    //QVERIFY(test.isValid());
}

void TestDxa::forearmTest()
{
    qDebug("forearm test");
}

void TestDxa::hipTest()
{
    qDebug("hip test");
}

void TestDxa::ivaImagingTest()
{
    qDebug("iva imaging test");
}

void TestDxa::wholeBodyScanTest()
{
    qDebug("whole body scan test");
}

QTEST_APPLESS_MAIN(TestDxa)

#include "tst_dxa.moc"
