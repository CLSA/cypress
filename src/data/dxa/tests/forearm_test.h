#ifndef FOREARM_TEST_H
#define FOREARM_TEST_H

#include <QJsonObject>

#include "data/test_base.h"

class ForearmTest : public TestBase
{
public:
    explicit ForearmTest();

    QString toString() const override;
    QJsonObject toJsonObject() const override;

    bool isValid() const override;
    void reset() override;
};

#endif // FOREARM_TEST_H
