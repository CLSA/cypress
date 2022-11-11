#ifndef APSPINESCANMANAGER_H
#define APSPINESCANMANAGER_H

#include "DXAManager.h"

class APSpineScanManager : public DXAManager
{
    Q_OBJECT
public:
    explicit APSpineScanManager(QObject *parent = nullptr);

    QMap<QString, QVariant> extractData() override;

    QString getName() override;
    QString getBodyPartName() override;
    Side getSide() override;
    quint8 getScanType() override;
    QString getRefType() override;
    QString getRefSource() override;
};

#endif // APSPINESCANMANAGER_H
