#include <QVariant>
#include <QString>
#include <QMap>
#include <QObject>

#include "managers/dxa/DXAManager.h"
#include "managers/dxa/HipScanManager.h"

HipScanManager::HipScanManager(QObject* parent)
    : DXAManager{parent}
{
    qDebug() << "Hip Scan Manager init";
}

// window invoking its run method
//
void HipScanManager::start()
{
    qDebug() << "HipScanManager::start";
    startDicomServer();
};


// retrieve a measurement from the device
//
void HipScanManager::measure()
{

};

// implementation of final clean up of device after disconnecting and all
// data has been retrieved and processed by any upstream classes
//
void HipScanManager::finish()
{
    qDebug() << "HipScanManager::end";
    endDicomServer();
};

QString HipScanManager::getName()
{
    switch(m_test.side) {
        case LEFT:
            return "L_HIP";
        case RIGHT:
            return "R_HIP";
        default:
            return "";
    }
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
    switch(m_test.side) {
        case LEFT:
            return 61;
        case RIGHT:
            return 71;
    }

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
    return QMap<QString, QVariant> {{}};
}


QJsonObject HipScanManager::toJsonObject() const
{
    return QJsonObject();
}

void HipScanManager::initializeModel()
{

}

void HipScanManager::updateModel()
{

}

void HipScanManager::setInputData(const QVariantMap& inputData)
{

}

void HipScanManager::clearData()
{

}

QMap<QString, QVariant> HipScanManager::extractScanAnalysisData(const QString& tableName)
{
    return QMap<QString, QVariant> {{}};
}

QMap<QString, QVariant> HipScanManager::computeTandZScores()
{
    return QMap<QString, QVariant> {{}};
}
