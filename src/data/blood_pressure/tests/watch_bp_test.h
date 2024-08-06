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

    bool isValid() const;
    void reset();
    void reinterpret();

    void updateAverage();

    void setCuffSize(const QString&);
    void setSide(const QString&);

    QString toString() const;
    QJsonObject toJsonObject() const;

    void fromJson(const QJsonObject&);

private:
    QList<QString> m_outputKeyList;
};

#endif // WATCH_BP_TEST_H
