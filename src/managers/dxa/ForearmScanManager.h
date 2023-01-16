#ifndef FOREARMSCANMANAGER_H
#define FOREARMSCANMANAGER_H

#include "DXAManager.h"
#include "data/dxa/tests/ForearmTest.h"

class ForearmScanManager : public DXAManager
{
public:
    explicit ForearmScanManager(QWidget *parent = nullptr);

    bool validateDicomFile(DcmFileFormat &loadedFileFormat) override;

    QVariantMap retrieveDeviceData() override;
    QVariantMap extractScanAnalysisData() override;
    QVariantMap computeTandZScores() override;

    QJsonObject toJsonObject() const override;

    void initializeModel() override;
    void updateModel() override;
    void setInputData(const QVariantMap& inputData) override;
    void clearData() override;
    QString getName() override;
    QString getBodyPartName() override;
    Side getSide() override;
    quint8 getScanType() override;
    QString getRefType() override;
    QString getRefSource() override;


private:
    ForearmTest m_test;
};

#endif // FOREARMSCANMANAGER_H
