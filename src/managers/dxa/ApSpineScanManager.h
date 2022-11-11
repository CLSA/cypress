#ifndef APSPINESCANMANAGER_H
#define APSPINESCANMANAGER_H

#include "DXAManager.h"
#include "data/dxa/tests/ApSpineTest.h"

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

    QMap<QString, QVariant> extractScanAnalysisData(const QString& tableName) override;
    QMap<QString, QVariant> computeTandZScores() override;

private:
    ApSpineTest m_test;
};

#endif // APSPINESCANMANAGER_H
