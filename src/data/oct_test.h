#ifndef OCT_TEST_H
#define OCT_TEST_H

#include "test_base.h"

class OCTTest : public TestBase
{
public:
    OCTTest();

    // TestBase interface
public:
    QString toString() const override;
    bool isValid() const override;
    QJsonObject toJsonObject() const override;
    void simulate() override;
    void reset() override;
    void reinterpret() override;
};

#endif // OCT_TEST_H
