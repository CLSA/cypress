#include "DXAManager.h"
#include <QMap>
#include <QVariant>
#include <QString>
#include <QSql>

DXAManager::DXAManager(QObject *parent)
    : ManagerBase{parent}
{

};

void DXAManager::loadSettings(const QSettings &)
{

};

void DXAManager::saveSettings(QSettings*) const
{

};

QMap<QString, QVariant> DXAManager::extractScanAnalysisData()
{
    // make connection to PatScan.mdb
    // query data

    return QMap<QString, QVariant>{{"", ""}};
}
