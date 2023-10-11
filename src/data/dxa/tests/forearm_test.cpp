#include <QDebug>

#include "forearm_test.h"



ForearmTest::ForearmTest()
{
}

void ForearmTest::reset()
{
    m_metaData.reset();
    m_measurementList.clear();
}

QString ForearmTest::toString() const
{
   return "";
}

QJsonObject ForearmTest::toJsonObject() const
{
    return QJsonObject();
}

