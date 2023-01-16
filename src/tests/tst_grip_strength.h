#ifndef TST_GRIP_STRENGTH_H
#define TST_GRIP_STRENGTH_H

#include <QTest>
#include <QJsonObject>

class TestGripStrength: public QObject
{
    Q_OBJECT

public:
    TestGripStrength();
    ~TestGripStrength();

private:
    QJsonObject expectedInput;
    QJsonObject expectedOutput;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase2();
};

#endif // TST_GRIP_STRENGTH_H
