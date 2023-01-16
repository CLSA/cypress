#ifndef TST_ULTRASOUND_H
#define TST_ULTRASOUND_H

#include <QTest>
#include <QJsonObject>

class TestUltrasound: public QObject
{
    Q_OBJECT

public:
    TestUltrasound();
    ~TestUltrasound();

private:
    QJsonObject expectedInput;
    QJsonObject expectedOutput;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase2();
};

#endif // TST_ULTRASOUND_H
