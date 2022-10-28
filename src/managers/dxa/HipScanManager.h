#ifndef HIPSCANMANAGER_H
#define HIPSCANMANAGER_H

#include "DXAManager.h"
#include <QObject>

class HipScanManager : public DXAManager
{
public:
    explicit HipScanManager(QObject *parent = nullptr);

    QMap<QString, QVariant> extractData() override;

    QString getName() override;
    QString getBodyPartName() override;
    Side getSide() override;
    quint8 getScanType() override;
    QString getRefType() override;
    QString getRefSource() override;
};

#endif // HIPSCANMANAGER_H
