#include "ForearmScanManager.h"
#include "data/DXATest.h"
#include <QMap>
#include <QVariant>
#include <QString>


ForearmScanManager::ForearmScanManager(QObject *parent)
    : DXAManager{parent}
{
}

QString ForearmScanManager::getName()
{
    switch (m_test.side)
    {
        case LEFT:
            return "L_FA";
        case RIGHT:
            return "R_FA";
    }

    return "R_FA";
}

QString ForearmScanManager::getBodyPartName()
{
   return "ARM";
}

Side ForearmScanManager::getSide()
{
    return m_test.side;
}

quint8 ForearmScanManager::getScanType()
{
    switch(m_test.side) {
        case LEFT:
            return 61;
        case RIGHT:
            return 71;
    }

    return 71;
}

QString ForearmScanManager::getRefType()
{
    return "R";
}

QString ForearmScanManager::getRefSource()
{
   return "Hologic";
}

QMap<QString, QVariant> ForearmScanManager::extractData()
{
    QMap<QString, QVariant> analysisData = extractScanAnalysisData();
    for (QString key: m_test.testKeys)
    {
       m_test.data[key] = analysisData[key];
    }

    return m_test.data;
}


