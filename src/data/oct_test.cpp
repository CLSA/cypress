#include "oct_test.h"

#include <QJsonObject>

OCTTest::OCTTest() {
}

QString OCTTest::toString() const
{
    return "OCTTest";
}

bool OCTTest::isValid() const
{
    return false;
}

QJsonObject OCTTest::toJsonObject() const
{
    return QJsonObject {};
}

void OCTTest::reset()
{

}

void OCTTest::reinterpret()
{

}
