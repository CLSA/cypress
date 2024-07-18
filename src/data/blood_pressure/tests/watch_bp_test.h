#ifndef WATCH_BP_TEST_H
#define WATCH_BP_TEST_H

#include "data/test_base.h"

#include <QJsonObject>

class WatchBPTest : public TestBase
{
public:
    WatchBPTest();

    // TestBase interface
public:
    QString toString() const;
    bool isValid() const;
    QJsonObject toJsonObject() const;
    void reset();
    void reinterpret();

private:
    QList<QString> m_outputKeyList;
};

#endif // WATCH_BP_TEST_H
