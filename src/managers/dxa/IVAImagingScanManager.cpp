#include "IVAImagingScanManager.h"
#include <QVariant>
#include <QString>

IVAImagingScanManager::IVAImagingScanManager(QObject *parent)
    : DXAManager{parent}
{

}
QString IVAImagingScanManager::getName()
{
    return "";
}

QString IVAImagingScanManager::getBodyPartName()
{
   return "";
}

Side IVAImagingScanManager::getSide()
{
    return m_test.side;
}

quint8 IVAImagingScanManager::getScanType()
{
    //switch(m_test.side) {
    //    case LEFT:
    //        return 61;
    //    case RIGHT:
    //        return 71;
    //}

    return -1;
}

QString IVAImagingScanManager::getRefType()
{
    return "";
}

QString IVAImagingScanManager::getRefSource()
{
   return "Hologic";
}

QMap<QString, QVariant> IVAImagingScanManager::extractData()
{
    //QMap<QString, QVariant> analysisData = extractScanAnalysisData();
    //for (QString key: m_test.testKeys)
    //{
    //   m_test.data[key] = analysisData[key];
    //}

    return QMap<QString, QVariant> {{}};
}
