#ifndef TST_CDTT_H
#define TST_CDTT_H

#include <QTest>
#include <QJsonObject>

class TestCDTT: public QObject
{
    Q_OBJECT

public:
    TestCDTT();
    ~TestCDTT();

private:
    QJsonObject expectedInput;
    QJsonObject expectedOutput;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase2();
};

#endif // TST_CDTT_H
