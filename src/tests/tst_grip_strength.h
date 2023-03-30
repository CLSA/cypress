#ifndef TST_GRIP_STRENGTH_H
#define TST_GRIP_STRENGTH_H

#include <QObject>

class TestGripStrength : public QObject
{
    Q_OBJECT

public:
    TestGripStrength();

private Q_SLOTS:
    void testInitialize();
    void testExtractExam();
};

#endif // TST_GRIP_STRENGTH_H
