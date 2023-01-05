#include <QtTest>

//#include "../data/retinal_camera/tests/RetinalCameraTest.h"

class RetinalCameraTest: public QObject
{
    Q_OBJECT

public:
    RetinalCameraTest();
    ~RetinalCameraTest();

private:
    QJsonObject expectedInput;
    QJsonObject expectedOutput;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase2();
};

RetinalCameraTest::RetinalCameraTest()
{
    qDebug("constructor");
}

RetinalCameraTest::~RetinalCameraTest()
{

    qDebug("destructor");
}

void RetinalCameraTest::initTestCase()
{
}

void RetinalCameraTest::cleanupTestCase()
{
}

void RetinalCameraTest::testCase1()
{
    RetinalCameraTest test;
    //test.readGripTestOptions();
    //test.readGripTestResults();
    //QVERIFY(test.isValid());

}

void RetinalCameraTest::testCase2()
{
    qDebug("test case 1");
}

QTEST_APPLESS_MAIN(RetinalCameraTest)

#include "tst_retinal_camera.moc"
