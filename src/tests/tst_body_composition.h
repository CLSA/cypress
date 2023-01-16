#ifndef TST_BODY_COMPOSITION_H
#define TST_BODY_COMPOSITION_H

#include <QTest>
#include <QJsonObject>

class TestBodyComposition: public QObject
{
    Q_OBJECT

public:
    TestBodyComposition();
    ~TestBodyComposition();

private:
    QJsonObject expectedInput;
    QJsonObject expectedOutput;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
};

#endif // TST_BODY_COMPOSITION_H
