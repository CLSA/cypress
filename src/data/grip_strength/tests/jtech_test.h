#ifndef JTECH_TEST_H
#define JTECH_TEST_H

#include "data/test_base.h"
#include <QJsonObject>

class JTechTest : public TestBase
{
public:
    JTechTest();

    // TestBase interface
public:
    QString toString() const;
    bool isValid() const;
    QJsonObject toJsonObject() const;
};

#endif // JTECH_TEST_H
