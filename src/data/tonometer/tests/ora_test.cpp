#include "ora_test.h"

ORATest::ORATest() {}

QString ORATest::toString() const
{
    return "ORATest";
}

bool ORATest::isValid() const
{
    return false;
}

QJsonObject ORATest::toJsonObject() const
{
    QJsonObject json;
    return json;
}
