#include "ApSpineScanManager.h"
#include <QMap>
#include <QVariant>
#include <QString>

APSpineScanManager::APSpineScanManager(QObject *parent)
    : DXAManager{parent}
{

}

QString APSpineScanManager::getName()
{
    return "";
}

QString APSpineScanManager::getBodyPartName()
{
   return "";
}

Side APSpineScanManager::getSide()
{
    return m_test.side;
}

quint8 APSpineScanManager::getScanType()
{
    //switch(m_test.side) {
    //    case LEFT:
    //        return 61;
    //    case RIGHT:
    //        return 71;
    //}

    return -1;
}

QString APSpineScanManager::getRefType()
{
    return "";
}

QString APSpineScanManager::getRefSource()
{
   return "Hologic";
}

QMap<QString, QVariant> APSpineScanManager::extractData()
{
    //QMap<QString, QVariant> analysisData = extractScanAnalysisData();
    //for (QString key: m_test.testKeys)
    //{
    //   m_test.data[key] = analysisData[key];
    //}

    return m_test.data;
}
