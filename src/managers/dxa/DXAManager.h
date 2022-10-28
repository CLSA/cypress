#ifndef DXAMANAGER_H
#define DXAMANAGER_H

#include "data/DXATest.h"
#include "managers/ManagerBase.h"

#include <QObject>
#include <QMap>
#include <QVariant>
#include <QString>

/*
 * Static ivar needed for computing T- and Z-scores. Map distinct BMD variable name(s) (eg., HTOT_BMD) for a given
 * PatScanDb table (eg., Hip) and the corresponding bonerange code in the RefScanDb ReferenceCurve table (eg., 123.).
 * Additional BMD variables and codes should be added here for other tables (ie., Spine).
 */
static QMap<QString, QString> ranges
{
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

class DXAManager : public ManagerBase
{
    Q_OBJECT
public:
    explicit DXAManager(QObject *parent = nullptr);

    virtual QString getName() = 0;
    virtual QString getBodyPartName() = 0;
    virtual Side getSide() = 0;
    virtual quint8 getScanType() = 0;
    virtual QString getRefType() = 0;
    virtual QString getRefSource() = 0;

    virtual QMap<QString, QVariant> extractData() = 0;

    void loadSettings(const QSettings &) override;
    void saveSettings(QSettings*) const override;

protected:
    QMap<QString, QVariant> extractScanAnalysisData();
    QMap<QString, QVariant> computeTandZScores();
    DXATest m_test;


signals:

};

#endif // DXAMANAGER_H
