#ifndef TST_WEIGH_SCALE_H
#define TST_WEIGH_SCALE_H

#include <QTest>
#include <QJsonObject>

class TestWeighScale: public QObject
{
    Q_OBJECT

public:
    TestWeighScale();
    ~TestWeighScale();

private:
    QJsonObject expectedInput;
    QJsonObject expectedOutput;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase2();
};

#endif // TST_WEIGH_SCALE_H
