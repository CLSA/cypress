#ifndef APSPINESCANMANAGER_H
#define APSPINESCANMANAGER_H

#include "DXAManager.h"
#include "data/dxa/tests/ApSpineTest.h"

class APSpineScanManager : public DXAManager
{
    Q_OBJECT
public:
    explicit APSpineScanManager(QObject *parent = nullptr);

    QMap<QString, QVariant> extractData(QStringList filePaths) override;

    virtual bool validateDicomFile(DcmFileFormat loadedFileFormat) override;

    void initializeModel() override;
    void updateModel() override;
    void setInputData(const QVariantMap& inputData) override;
    void clearData() override;
    QJsonObject toJsonObject() const override;

    QString getName() override;
    QString getBodyPartName() override;
    Side getSide() override;
    quint8 getScanType() override;
    QString getRefType() override;
    QString getRefSource() override;

    QMap<QString, QVariant> extractScanAnalysisData(const QString& tableName) override;
    QMap<QString, QVariant> computeTandZScores() override;

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
    ApSpineTest m_test;
};

#endif // APSPINESCANMANAGER_H
