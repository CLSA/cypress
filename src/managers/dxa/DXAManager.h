#ifndef DXAMANAGER_H
#define DXAMANAGER_H

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


enum Side {
    LEFT,
    RIGHT
};

class DXAManager : public ManagerBase
{
    Q_OBJECT
public:
    explicit DXAManager(QObject *parent = nullptr);

    const static QMap<QString, QString> ranges;

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
    QMap<QString, QVariant> extractScanAnalysisData(const QString& tableName);
    QMap<QString, QVariant> computeTandZScores();
};

#endif // DXAMANAGER_H
