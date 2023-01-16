#ifndef TST_FRAX_H
#define TST_FRAX_H

#include <QTest>
#include <QJsonObject>

class TestFrax: public QObject
{
    Q_OBJECT

public:
    TestFrax();
    ~TestFrax();

private:
    QJsonObject expectedInput;
    QJsonObject expectedOutput;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase2();
};

#endif // TST_FRAX_H
