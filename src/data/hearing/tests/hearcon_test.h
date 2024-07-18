#ifndef HEARCON_TEST_H
#define HEARCON_TEST_H

#include "data/test_base.h"

#include <QJsonObject>

class HearconTest : public TestBase
{
public:
    HearconTest();
    ~HearconTest() = default;

    // TestBase interface
public:
    void fromJsonFile(const QString& filePath);

    QString toString() const override;
    bool isValid() const override;
    QJsonObject toJsonObject() const override;

    void reset() override;
    void reinterpret() override;
};

#endif // HEARCON_TEST_H
