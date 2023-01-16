#ifndef TST_RETINAL_CAMERA_H
#define TST_RETINAL_CAMERA_H

#include <QTest>
#include <QJsonObject>

class TestRetinalCamera: public QObject
{
    Q_OBJECT

public:
    TestRetinalCamera();
    ~TestRetinalCamera();

private:
    QJsonObject expectedInput;
    QJsonObject expectedOutput;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase2();
};

#endif // TST_RETINAL_CAMERA_H
