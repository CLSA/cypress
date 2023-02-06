#ifndef APSPINESCANMANAGER_H
#define APSPINESCANMANAGER_H

#include "DXAManager.h"
#include "data/dxa/tests/ApSpineTest.h"

class APSpineScanManager : public DXAManager
{
    Q_OBJECT
public:
    explicit APSpineScanManager();

    virtual bool validateDicomFile(DcmFileFormat &loadedFileFormat) override;

    QVariantMap retrieveDeviceData() override;
    QVariantMap extractScanAnalysisData() override;
    QVariantMap computeTandZScores() override;

    void clearData() override;

    Side getSide() override;
    QString getName() override;
    QString getBodyPartName() override;
    quint8 getScanType() override;
    QString getRefType() override;
    QString getRefSource() override;

public slots:
    void finish() override;

private:
    ApSpineTest m_test;
};

#endif // APSPINESCANMANAGER_H
