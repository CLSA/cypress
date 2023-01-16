#ifndef TST_TONOMETER_H
#define TST_TONOMETER_H

#include <QTest>
#include <QJsonObject>

class TestTonometer: public QObject
{
    Q_OBJECT

public:
    TestTonometer();
    ~TestTonometer();

private:
    QJsonObject expectedInput;
    QJsonObject expectedOutput;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase2();
};

#endif // TST_TONOMETER_H
