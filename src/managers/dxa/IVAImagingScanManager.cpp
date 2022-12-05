#include "IVAImagingScanManager.h"
#include <QVariant>
#include <QString>

IVAImagingScanManager::IVAImagingScanManager(QObject *parent)
    : DXAManager{parent}
{

}

void IVAImagingScanManager::start()
{
    if (m_verbose)
        qDebug() << "IVAImagingScanManager::start";

    startDicomServer();
};

void IVAImagingScanManager::measure()
{

};

void IVAImagingScanManager::finish()
{
    if (m_verbose)
        qDebug() << "IVAImagingScanManager::end";

    endDicomServer();
};

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

QMap<QString, QVariant> IVAImagingScanManager::extractData(QStringList filePaths)
{
    //QMap<QString, QVariant> analysisData = extractScanAnalysisData();
    //for (QString key: m_test.testKeys)
    //{
    //   m_test.data[key] = analysisData[key];
    //}

    return QMap<QString, QVariant> {{}};
}

bool IVAImagingScanManager::validateDicomFile(DcmFileFormat fileFormat)
{
   return false;
}

QMap<QString, QVariant> IVAImagingScanManager::extractScanAnalysisData(const QString& tableName)
{
    // make connection to PatScan.mdb
    // query data

    return QMap<QString, QVariant>{{"", ""}};
}

QMap<QString, QVariant> IVAImagingScanManager::computeTandZScores()
{
    return QMap<QString, QVariant> {{}};
}


QJsonObject IVAImagingScanManager::toJsonObject() const
{
    return QJsonObject();
}

void IVAImagingScanManager::initializeModel()
{

}

void IVAImagingScanManager::updateModel()
{

}

void IVAImagingScanManager::setInputData(const QVariantMap& inputData)
{

}

void IVAImagingScanManager::clearData()
{

}
