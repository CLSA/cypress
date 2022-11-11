#include "DXAManager.h"
#include "HipScanManager.h"
#include <QVariant>
#include <QString>
#include <QObject>

HipScanManager::HipScanManager(QObject* parent)
    : DXAManager{parent}
{

}

QString HipScanManager::getName()
{
    return "";
}

QString HipScanManager::getBodyPartName()
{
   return "";
}

Side HipScanManager::getSide()
{
    return m_test.side;
}

quint8 HipScanManager::getScanType()
{
    //switch(m_test.side) {
    //    case LEFT:
    //        return 61;
    //    case RIGHT:
    //        return 71;
    //}

    return -1;
}

QString HipScanManager::getRefType()
{
    return "";
}

QString HipScanManager::getRefSource()
{
   return "Hologic";
}

QMap<QString, QVariant> HipScanManager::extractData()
{
    //QMap<QString, QVariant> analysisData = extractScanAnalysisData();
    //for (QString key: m_test.testKeys)
    //{
    //   m_test.data[key] = analysisData[key];
    //}

    return m_test.data;
}
