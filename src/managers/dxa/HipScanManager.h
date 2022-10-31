#ifndef HIPSCANMANAGER_H
#define HIPSCANMANAGER_H

#include <QObject>

#include "managers/dxa/DXAManager.h"
#include "data/dxa/tests/HipTest.h"

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

private:
    HipTest m_test;
};

#endif // HIPSCANMANAGER_H
