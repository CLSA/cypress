#ifndef FOREARMSCANMANAGER_H
#define FOREARMSCANMANAGER_H

#include "DXAManager.h"
#include "data/dxa/tests/ForearmTest.h"

class ForearmScanManager : public DXAManager
{
public:
    explicit ForearmScanManager();

    bool validateDicomFile(DcmFileFormat &loadedFileFormat) override;

    QVariantMap retrieveDeviceData() override;
    QVariantMap extractScanAnalysisData() override;
    QVariantMap computeTandZScores() override;

    void clearData() override;

    QString getName() override;
    QString getBodyPartName() override;
    Side getSide() override;
    quint8 getScanType() override;
    QString getRefType() override;
    QString getRefSource() override;


public slots:
    void finish() override;

private:
    ForearmTest m_test;
};

#endif // FOREARMSCANMANAGER_H
