#ifndef TST_DXA_H
#define TST_DXA_H

#include <QTest>
#include <QJsonObject>

class TestDxa: public QObject
{
    Q_OBJECT

public:
    TestDxa();
    ~TestDxa();

private:
    QJsonObject expectedInput;
    QJsonObject expectedOutput;

private slots:
    void initTestCase();
    void cleanupTestCase();

    void apSpineTest();
    void forearmTest();
    void hipTest();
    void ivaImagingTest();
    void wholeBodyScanTest();
};

#endif // TST_DXA_H

