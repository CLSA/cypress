#ifndef TST_AUDIOMETER_H
#define TST_AUDIOMETER_H

#include <QTest>
#include <QJsonObject>

class TestAudiometer: public QObject
{
    Q_OBJECT

public:
    TestAudiometer();
    ~TestAudiometer();

private:
    QJsonObject expectedInput;
    QJsonObject expectedOutput;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
};


#endif // TST_AUDIOMETER_H
