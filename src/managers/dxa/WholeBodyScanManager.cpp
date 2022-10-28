#include "WholeBodyScanManager.h"
#include <QVariant>
#include <QString>

WholeBodyScanManager::WholeBodyScanManager(QObject *parent)
    : DXAManager{parent}
{

}


QString WholeBodyScanManager::getName()
{
    return "";
}

QString WholeBodyScanManager::getBodyPartName()
{
   return "";
}

Side WholeBodyScanManager::getSide()
{
    return m_test.side;
}

quint8 WholeBodyScanManager::getScanType()
{
    //switch(m_test.side) {
    //    case LEFT:
    //        return 61;
    //    case RIGHT:
    //        return 71;
    //}

    return -1;
}

QString WholeBodyScanManager::getRefType()
{
    return "";
}

QString WholeBodyScanManager::getRefSource()
{
   return "Hologic";
}

QMap<QString, QVariant> WholeBodyScanManager::extractData()
{
    //QMap<QString, QVariant> analysisData = extractScanAnalysisData();
    //for (QString key: m_test.testKeys)
    //{
    //   m_test.data[key] = analysisData[key];
    //}

    return m_test.data;
}
