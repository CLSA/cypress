#ifndef IVAIMAGINGSCANMANAGER_H
#define IVAIMAGINGSCANMANAGER_H

#include "DXAManager.h"
#include "data/dxa/tests/IVAImagingTest.h"

class IVAImagingScanManager : public DXAManager
{
public:
    explicit IVAImagingScanManager(QObject *parent = nullptr);

    void initializeModel() override;
    void updateModel() override;
    void setInputData(const QVariantMap& inputData) override;
    void clearData() override;
    QJsonObject toJsonObject() const override;

    bool validateDicomFile(DcmFileFormat loadedFileFormat) override;
    QMap<QString, QVariant> extractData(QStringList filePaths) override;
    QMap<QString, QVariant> computeTandZScores() override;
    QMap<QString, QVariant> extractScanAnalysisData(const QString& tableName) override;

    QString getName() override;
    QString getBodyPartName() override;
    Side getSide() override;
    quint8 getScanType() override;
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
    IVAImagingTest m_test;
};

#endif // IVAIMAGINGSCANMANAGER_H
