#ifndef OCT_TEST_H
#define OCT_TEST_H

#include "data/test_base.h"

class OCTTest : public TestBase
{
public:
    OCTTest();

    // TestBase interface
public:
    QString toString() const;
    bool isValid() const;
    QJsonObject toJsonObject() const;
    void reinterpret();
};

#endif // OCT_TEST_H
