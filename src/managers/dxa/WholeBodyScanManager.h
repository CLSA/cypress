#ifndef WHOLEBODYSCANMANAGER_H
#define WHOLEBODYSCANMANAGER_H

#include "DXAManager.h"
#include "data/dxa/tests/WholeBodyScanTest.h"

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

    QMap<QString, QVariant> extractScanAnalysisData(const QString& tableName) override;
    QMap<QString, QVariant> computeTandZScores() override;


private:
    WholeBodyScanTest m_test;

protected slots:
    void dicomFilesReceived(QStringList paths) override;
};

#endif // WHOLEBODYSCANMANAGER_H
