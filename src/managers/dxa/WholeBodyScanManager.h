#ifndef WHOLEBODYSCANMANAGER_H
#define WHOLEBODYSCANMANAGER_H

#include "DXAManager.h"

class WholeBodyScanManager : public DXAManager
{
public:
    explicit WholeBodyScanManager(QObject *parent = nullptr);

    QMap<QString, QVariant> extractData() override;

    QString getName() override;
    QString getBodyPartName() override;
    Side getSide() override;
    quint8 getScanType() override;
    QString getRefType() override;
    QString getRefSource() override;
};

#endif // WHOLEBODYSCANMANAGER_H
