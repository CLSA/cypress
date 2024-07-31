#include "easyone_test.h"

EasyoneTest::EasyoneTest() {}

QString EasyoneTest::toString() const
{
    return "EasyoneTest";
}

bool EasyoneTest::isValid() const
{
    return true;
}

QJsonObject EasyoneTest::toJsonObject() const
{
    QJsonObject json {};
    return json;
}

void EasyoneTest::reset()
{
    qDebug() << "EasyoneTest::reset";
}

void EasyoneTest::reinterpret()
{
    qDebug() << "EasyoneTest::reinterpret";
}
