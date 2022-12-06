#ifndef WHOLEBODYSCANMANAGER_H
#define WHOLEBODYSCANMANAGER_H

#include "DXAManager.h"
#include "data/dxa/tests/WholeBodyScanTest.h"

class WholeBodyScanManager : public DXAManager
{
public:
    explicit WholeBodyScanManager(QObject *parent = nullptr);

    bool validateDicomFile(DcmFileFormat loadedFileFormat) override;

    void initializeModel() override;
    void updateModel() override;
    void setInputData(const QVariantMap& inputData) override;
    void clearData() override;

    QJsonObject toJsonObject() const override;
    QVariantMap extractScanAnalysisData() override;
    QVariantMap computeTandZScores() override;

    Side getSide() override;
    quint8 getScanType() override;

    QString getRefType() override;
    QString getRefSource() override;
    QString getName() override;
    QString getBodyPartName() override;

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
    WholeBodyScanTest m_test;
};

#endif // WHOLEBODYSCANMANAGER_H
