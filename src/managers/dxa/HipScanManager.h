#ifndef HIPSCANMANAGER_H
#define HIPSCANMANAGER_H

#include <QObject>

#include "managers/dxa/DXAManager.h"
#include "data/dxa/tests/HipTest.h"

class HipScanManager : public DXAManager
{
    Q_OBJECT

public:
    explicit HipScanManager(QObject *parent = nullptr);

    void initializeModel() override;
    void updateModel() override;
    void setInputData(const QVariantMap& inputData) override;
    void clearData() override;

    bool validateDicomFile(DcmFileFormat loadedFileFormat) override;

    QVariantMap extractScanAnalysisData() override;
    QVariantMap computeTandZScores() override;
    QJsonObject toJsonObject() const override;

    Side getSide() override;
    quint8 getScanType() override;
    QString getName() override;
    QString getBodyPartName() override;
    QString getRefType() override;
    QString getRefSource() override;

public slots:
    // what the manager does in response to the main application
    // window invoking its run method
    //
    void start() override;

    // retrieve a measurement from the device
    //
    void measure() override;

    // implementation of final clean up of device after disconnecting and all
    // data has been retrieved and processed by any upstream classes
    //
    void finish() override;

private:
    HipTest m_test;
};

#endif // HIPSCANMANAGER_H
