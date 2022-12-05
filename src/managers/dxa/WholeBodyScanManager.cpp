#include "WholeBodyScanManager.h"
#include <QVariant>
#include <QString>

WholeBodyScanManager::WholeBodyScanManager(QObject *parent)
    : DXAManager{parent}
{
}

void WholeBodyScanManager::start()
{
    if (m_verbose)
        qDebug() << "WholeBodyScanManager::end";

    startDicomServer();
};

void WholeBodyScanManager::measure()
{

};

void WholeBodyScanManager::finish()
{
    if (m_verbose)
        qDebug() << "WholeBodyScanManager::end";

    endDicomServer();
};

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

bool WholeBodyScanManager::validateDicomFile(DcmFileFormat file)
{
    return false;
}

QMap<QString, QVariant> WholeBodyScanManager::extractData(QStringList filePaths)
{
    //QMap<QString, QVariant> analysisData = extractScanAnalysisData();
    //for (QString key: m_test.testKeys)
    //{
    //   m_test.data[key] = analysisData[key];
    //}

    //return m_test.data;
    return QMap<QString, QVariant> {{}};
}

void WholeBodyScanManager::dicomFilesReceived(QStringList paths)
{
    qDebug() << "WholeBodyScanManager" << paths;
}


QMap<QString, QVariant> WholeBodyScanManager::extractScanAnalysisData(const QString& tableName)
{
    // make connection to PatScan.mdb
    // query data

    return QMap<QString, QVariant>{{"", ""}};
}

QMap<QString, QVariant> WholeBodyScanManager::computeTandZScores()
{
    return QMap<QString, QVariant> {{}};
}

QJsonObject WholeBodyScanManager::toJsonObject() const
{
    return QJsonObject();
}

void WholeBodyScanManager::initializeModel()
{

}

void WholeBodyScanManager::updateModel()
{

}

void WholeBodyScanManager::setInputData(const QVariantMap& inputData)
{

}

void WholeBodyScanManager::clearData()
{

}
