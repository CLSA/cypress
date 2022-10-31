#include "IVAImagingTest.h"

IVAImagingTest::IVAImagingTest()
{

}

bool IVAImagingTest::isValid() const
{
    return false;
}

void IVAImagingTest::reset()
{
    m_metaData.reset();
    m_measurementList.clear();
}

QString IVAImagingTest::toString() const
{
   return "";
}

QJsonObject IVAImagingTest::toJsonObject() const
{
    return QJsonObject();
}

const QList<QString> IVAImagingTest::m_outputKeyList = {

};

const QList<QString> IVAImagingTest::m_inputKeyList = {

};


