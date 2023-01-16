#ifndef TST_EMR_H
#define TST_EMR_H

#include <QTest>
#include <QJsonObject>

class TestEmr: public QObject
{
    Q_OBJECT

public:
    TestEmr();
    ~TestEmr();

private:
    QJsonObject expectedInput;
    QJsonObject expectedOutput;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase2();
};

#endif // TST_EMR_H
