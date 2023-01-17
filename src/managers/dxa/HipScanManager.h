#ifndef HIPSCANMANAGER_H
#define HIPSCANMANAGER_H

#include <QObject>

#include "managers/dxa/DXAManager.h"
#include "data/dxa/tests/HipTest.h"

class HipScanManager : public DXAManager
{
    Q_OBJECT

public:
    explicit HipScanManager(QWidget *parent = nullptr);

    void clearData() override;

    bool validateDicomFile(DcmFileFormat &loadedFileFormat) override;

    QVariantMap retrieveDeviceData() override;
    QVariantMap extractScanAnalysisData() override;
    QVariantMap computeTandZScores() override;

    Side getSide() override;
    quint8 getScanType() override;
    QString getName() override;
    QString getBodyPartName() override;
    QString getRefType() override;
    QString getRefSource() override;

public slots:
    void finish() override;

private:
    HipTest m_test;
};

#endif // HIPSCANMANAGER_H
