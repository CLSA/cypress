#include "ApSpineScanManager.h"
#include <QMap>
#include <QVariant>
#include <QString>

APSpineScanManager::APSpineScanManager(QObject *parent)
    : DXAManager{parent}
{

}

void APSpineScanManager::start()
{
    if (m_verbose)
        qDebug() << "WholeBodyScanManager::end";

    startDicomServer();
};

void APSpineScanManager::measure()
{

};

void APSpineScanManager::finish()
{
    if (m_verbose)
        qDebug() << "WholeBodyScanManager::end";

    endDicomServer();
};

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

bool APSpineScanManager::validateDicomFile(DcmFileFormat loadedFileFormat)
{
    return false;
}

QMap<QString, QVariant> APSpineScanManager::extractData(QStringList filePaths)
{
    //QMap<QString, QVariant> analysisData = extractScanAnalysisData();
    //for (QString key: m_test.testKeys)
    //{
    //   m_test.data[key] = analysisData[key];
    //}

    return QMap<QString, QVariant> {{}};
}

QMap<QString, QVariant> APSpineScanManager::extractScanAnalysisData(const QString& tableName)
{
    return QMap<QString, QVariant> {{}};
}

QMap<QString, QVariant> APSpineScanManager::computeTandZScores()
{
    return QMap<QString, QVariant> {{}};
}

QJsonObject APSpineScanManager::toJsonObject() const
{
    return QJsonObject();
}

void APSpineScanManager::initializeModel()
{

}

void APSpineScanManager::updateModel()
{

}

void APSpineScanManager::setInputData(const QVariantMap& inputData)
{

}

void APSpineScanManager::clearData()
{

}
