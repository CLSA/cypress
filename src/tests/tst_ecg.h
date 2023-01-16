#ifndef TST_ECG_H
#define TST_ECG_H

#include <QTest>
#include <QJsonObject>

class TestEcg: public QObject
{
    Q_OBJECT

public:
    TestEcg();
    ~TestEcg();

private:
    QJsonObject expectedInput;
    QJsonObject expectedOutput;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase2();
};

#endif // TST_ECG_H
