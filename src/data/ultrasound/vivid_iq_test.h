#ifndef VIVID_IQ_TEST_H
#define VIVID_IQ_TEST_H

#include "data/test_base.h"

#include <QJsonObject>

class VividIQTest : public TestBase
{
public:
    VividIQTest();

    // TestBase interface
public:
    QString toString() const;
    bool isValid() const;
    QJsonObject toJsonObject() const;
};

#endif // VIVID_IQ_TEST_H
