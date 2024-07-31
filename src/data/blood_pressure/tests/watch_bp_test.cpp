#include "watch_bp_test.h"

WatchBPTest::WatchBPTest()
{
    m_outputKeyList << "Gender";
    m_outputKeyList << "Age";

    m_outputKeyList << "DOB";

    m_outputKeyList << "SYS";
    m_outputKeyList << "DIA";
    m_outputKeyList << "DaySYS";
    m_outputKeyList << "DayDIA";
    m_outputKeyList << "NightSYS";
    m_outputKeyList << "NightDIA";
}


bool WatchBPTest::isValid() const
{
    return false;
}

QJsonObject WatchBPTest::toJsonObject() const
{
    QJsonObject json{};

    // lorem ipsum todo
    return json;
}

void WatchBPTest::reset()
{

}

void WatchBPTest::reinterpret()
{

}

QString WatchBPTest::toString() const
{
    return "";
}
