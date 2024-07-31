#ifndef EASYONE_TEST_H
#define EASYONE_TEST_H

#include "data/test_base.h"

#include <QJsonObject>

class EasyoneTest : public TestBase
{
public:
    EasyoneTest();

    // TestBase interface
public:
    QString toString() const override;
    bool isValid() const override;
    QJsonObject toJsonObject() const override;
    void reset() override;
    void reinterpret() override;
};

#endif // EASYONE_TEST_H
