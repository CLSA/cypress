#ifndef ORA_TEST_H
#define ORA_TEST_H

#include "data/test_base.h"

#include <QJsonObject>

class ORATest : public TestBase
{
public:
    ORATest();

    // TestBase interface
public:
    QString toString() const;
    bool isValid() const;
    QJsonObject toJsonObject() const;
};

#endif // ORA_TEST_H
