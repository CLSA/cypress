#include "oct_test.h"

#include <QJsonObject>

OCTTest::OCTTest()
{

}

QString OCTTest::toString()
{
    return "";
}

bool OCTTest::isValid()
{
    return true;
}

QJsonObject OCTTest::toJsonObject()
{
    return TestBase::toJsonObject();
}

void OCTTest::reinterpret()
{

}
