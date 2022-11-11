#include "managers/dxa/DXAManager.h"
#include <QMap>
#include <QVariant>
#include <QString>
#include <QSql>

const QMap<QString, QString> DXAManager::ranges = {
    // forearm
    { "RU13TOT_BMD",    "1.." },
    { "RUMIDTOT_BMD",   ".2." },
    { "RUUDTOT_BMD",    "..3" },
    { "RUTOT_BMD",      "123" },
    { "R_13_BMD",       "R.." },
    { "R_MID_BMD",      ".R." },
    { "R_UD_BMD",       "..R" },
    { "RTOT_BMD",       "RRR" },
    { "U_13_BMD",       "U.." },
    { "U_MID_BMD",      ".U." },
    { "U_UD_BMD",       "..U" },
    { "UTOT_BMD",       "UUU" },

    // whole body
    { "WBTOT_BMD",      "NULL" },

    // hip
    { "NECK_BMD",       "1..." },
    { "TROCH_BMD",      ".2.." },
    { "INTER_BMD",      "..3." },
    { "WARDS_BMD",      "...4" },
    { "HTOT_BMD",       "123." },

    // ap lumbar spine
    { "L1_BMD",         "1..." },
    { "L2_BMD",         ".2.." },
    { "L3_BMD",         "..3." },
    { "L4_BMD",         "...4" },
    { "TOT_BMD",        "1234" },
    { "TOT_L1_BMD",     "1..." },
    { "TOT_L2_BMD",     ".2.." },
    { "TOT_L3_BMD",     "..3." },
    { "TOT_L4_BMD",     "...4" },
    { "TOT_L1L2_BMD",   "12.." },
    { "TOT_L1L3_BMD",   "1.3." },
    { "TOT_L1L4_BMD",   "1..4" },
    { "TOT_L2L3_BMD",   ".23." },
    { "TOT_L2L4_BMD",   ".2.4" },
    { "TOT_L3L4_BMD",   "..34" },
    { "TOT_L1L2L3_BMD", "123." },
    { "TOT_L1L2L4_BMD", "12.4" },
    { "TOT_L1L3L4_BMD", "1.34" },
    { "TOT_L2L3L4_BMD", ".234" },
};

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

QMap<QString, QVariant> DXAManager::extractScanAnalysisData(const QString& tableName)
{
    // make connection to PatScan.mdb
    // query data

    return QMap<QString, QVariant>{{"", ""}};
}
