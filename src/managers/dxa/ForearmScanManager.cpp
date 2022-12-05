#include "ForearmScanManager.h"
#include <QMap>
#include <QVariant>
#include <QString>


ForearmScanManager::ForearmScanManager(QObject *parent)
    : DXAManager{parent}
{
}

void ForearmScanManager::start()
{
    qDebug() << "ForearmScanManager::start";
    startDicomServer();
};

void ForearmScanManager::measure()
{

};

void ForearmScanManager::finish()
{
    qDebug() << "ForearmScanManager::end";
    endDicomServer();
};

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

bool ForearmScanManager::validateDicomFile(DcmFileFormat loadedFileFormat)
{
    return false;
}

QMap<QString, QVariant> ForearmScanManager::extractData(QStringList filePaths)
{
    //QMap<QString, QVariant> analysisData = extractScanAnalysisData();
    //for (QString key: m_test.testKeys)
    //{
    //   m_test.data[key] = analysisData[key];
    //}

    //return m_test.data;
    return QMap<QString, QVariant> {{}};
}

QMap<QString, QVariant> ForearmScanManager::extractScanAnalysisData(const QString& tableName)
{
    // make connection to PatScan.mdb
    // query data

    return QMap<QString, QVariant>{{"", ""}};
}

QMap<QString, QVariant> ForearmScanManager::computeTandZScores()
{
    return QMap<QString, QVariant> {{}};
}

QJsonObject ForearmScanManager::toJsonObject() const
{
    return QJsonObject();
}

void ForearmScanManager::initializeModel()
{

}

void ForearmScanManager::updateModel()
{

}

void ForearmScanManager::setInputData(const QVariantMap& inputData)
{

}

void ForearmScanManager::clearData()
{

}

