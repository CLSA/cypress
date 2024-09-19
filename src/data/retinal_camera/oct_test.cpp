#include "oct_test.h"

#include <QJsonObject>

OCTTest::OCTTest()
{

}

QString OCTTest::toString() const
{
    return "";
}

bool OCTTest::isValid() const
{
    return true;
}

QJsonObject OCTTest::toJsonObject() const
{
    return TestBase::toJsonObject();
}

void OCTTest::reinterpret()
{

}
