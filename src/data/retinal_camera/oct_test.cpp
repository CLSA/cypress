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
    if (m_measurementList.length() == 2) {
        return true;
    }

    return false;
}

QJsonObject OCTTest::toJsonObject() const
{
    return TestBase::toJsonObject();
}

void OCTTest::reinterpret()
{

}
