#ifndef DXAMANAGER_H
#define DXAMANAGER_H

#include "../ManagerBase.h"

#include <QObject>
#include <QMap>
#include <QVariant>
#include <QString>

#include "./dicom/DicomSCP.h"
#include "dcmtk/dcmdata/dcfilefo.h"
/*
 * Static ivar needed for computing T- and Z-scores. Map distinct BMD variable name(s) (eg., HTOT_BMD) for a given
 * PatScanDb table (eg., Hip) and the corresponding bonerange code in the RefScanDb ReferenceCurve table (eg., 123.).
 * Additional BMD variables and codes should be added here for other tables (ie., Spine).
 */


enum Side {
    LEFT,
    RIGHT,
    BOTH
};

class DXAManager : public ManagerBase
{
    Q_OBJECT
public:
    DXAManager(QWidget *parent = nullptr);
    ~DXAManager();

    const static QMap<QString, QString> ranges;

    QJsonObject scanAnalysisJson;
    QJsonObject scoresJson;

    DicomSCP* m_dicomSCP;

    QList<DcmFileFormat> validatedDicomFiles;
    QList<DcmFileFormat> getValidatedFiles(QStringList filePaths);

    QVariantMap getParticipantData();

    virtual Side getSide() = 0;
    virtual quint8 getScanType() = 0;

    virtual bool validateDicomFile(DcmFileFormat &loadedFileFormat) = 0;
    virtual QMap<QString, QVariant> retrieveDeviceData() = 0;
    virtual QMap<QString, QVariant> extractScanAnalysisData() = 0;
    virtual QMap<QString, QVariant> computeTandZScores() = 0;

    virtual QString getName() = 0;
    virtual QString getBodyPartName() = 0;
    virtual QString getRefType() = 0;
    virtual QString getRefSource() = 0;

    void dicomFilesReceived(QStringList paths);

    bool startDicomServer();
    bool endDicomServer();

    void loadSettings(const QSettings &) override;
    void saveSettings(QSettings*) const override;

    bool isCorrectDicom(DcmFileFormat &file);
    bool isCompleteDicom(DcmFileFormat &file);

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

protected slots:
    void dicomServerExitNormal();
    void dicomServerExitCrash();
};

#endif // DXAMANAGER_H
