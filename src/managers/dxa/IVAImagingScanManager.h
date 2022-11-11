#ifndef IVAIMAGINGSCANMANAGER_H
#define IVAIMAGINGSCANMANAGER_H

#include "DXAManager.h"
#include "data/dxa/tests/IVAImagingTest.h"

class IVAImagingScanManager : public DXAManager
{
public:
    explicit IVAImagingScanManager(QObject *parent = nullptr);

    QMap<QString, QVariant> extractData() override;

    QString getName() override;
    QString getBodyPartName() override;
    Side getSide() override;
    quint8 getScanType() override;
    QString getRefType() override;
    QString getRefSource() override;

    QMap<QString, QVariant> computeTandZScores() override;
    QMap<QString, QVariant> extractScanAnalysisData(const QString& tableName) override;
private:
    IVAImagingTest m_test;
};

#endif // IVAIMAGINGSCANMANAGER_H
