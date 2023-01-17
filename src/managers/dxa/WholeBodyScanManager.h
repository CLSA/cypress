#ifndef WHOLEBODYSCANMANAGER_H
#define WHOLEBODYSCANMANAGER_H

#include "DXAManager.h"
#include "data/dxa/tests/WholeBodyScanTest.h"

class WholeBodyScanManager : public DXAManager
{
public:
    explicit WholeBodyScanManager(QWidget *parent = nullptr);

    bool validateDicomFile(DcmFileFormat &loadedFileFormat) override;

    void clearData() override;

    QVariantMap retrieveDeviceData() override;
    QVariantMap extractScanAnalysisData() override;
    QVariantMap computeTandZScores() override;

    Side getSide() override;
    quint8 getScanType() override;

    QString getRefType() override;
    QString getRefSource() override;
    QString getName() override;
    QString getBodyPartName() override;

public slots:
    void finish() override;

private:
    WholeBodyScanTest m_test;
};

#endif // WHOLEBODYSCANMANAGER_H
