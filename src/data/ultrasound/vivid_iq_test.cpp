#include "vivid_iq_test.h"

VividIQTest::VividIQTest() {}

QString VividIQTest::toString() const
{
    return "";
}

bool VividIQTest::isValid() const
{
    return true;
}

QJsonObject VividIQTest::toJsonObject() const
{
    return TestBase::toJsonObject();
}
