#ifndef IVAIMAGINGSCANMANAGER_H
#define IVAIMAGINGSCANMANAGER_H

#include "DXAManager.h"
#include "data/dxa/tests/IVAImagingTest.h"

class IVAImagingScanManager : public DXAManager
{
public:
    explicit IVAImagingScanManager(QWidget *parent = nullptr);

    void clearData() override;

    bool validateDicomFile(DcmFileFormat &loadedFileFormat) override;

    QVariantMap retrieveDeviceData() override;
    QVariantMap computeTandZScores() override;
    QVariantMap extractScanAnalysisData() override;

    QString getName() override;
    QString getBodyPartName() override;
    Side getSide() override;
    quint8 getScanType() override;
    QString getRefType() override;
    QString getRefSource() override;

private:
    IVAImagingTest m_test;

    bool isDicomFile1(DcmFileFormat &loadedFileFormat);
    bool isDicomFile2(DcmFileFormat &loadedFileFormat);
    bool isDicomFile3(DcmFileFormat &loadedFileFormat);
};

#endif // IVAIMAGINGSCANMANAGER_H
