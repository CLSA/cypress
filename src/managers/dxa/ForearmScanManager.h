#ifndef FOREARMSCANMANAGER_H
#define FOREARMSCANMANAGER_H

#include "DXAManager.h"
#include "data/DXATest.h"
#include "data/ForearmTest.h"

class ForearmScanManager : public DXAManager
{
public:
    explicit ForearmScanManager(QObject *parent = nullptr);

    QMap<QString, QVariant> extractData() override;

    QString getName() override;
    QString getBodyPartName() override;
    Side getSide() override;
    quint8 getScanType() override;
    QString getRefType() override;
    QString getRefSource() override;

protected:
    ForearmTest m_test;


private:
};

#endif // FOREARMSCANMANAGER_H
