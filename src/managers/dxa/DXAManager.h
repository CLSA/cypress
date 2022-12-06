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
    RIGHT
};

class DXAManager : public ManagerBase
{
    Q_OBJECT
public:
    DXAManager(QObject *parent = nullptr);
    ~DXAManager();

    const static QMap<QString, QString> ranges;

    QList<DcmFileFormat> getValidatedFiles(QStringList filePaths);

    virtual bool validateDicomFile(DcmFileFormat loadedFileFormat) = 0;
    virtual Side getSide() = 0;
    virtual quint8 getScanType() = 0;
    virtual QMap<QString, QVariant> extractScanAnalysisData() = 0;
    virtual QMap<QString, QVariant> computeTandZScores() = 0;

    virtual QString getName() = 0;
    virtual QString getBodyPartName() = 0;
    virtual QString getRefType() = 0;
    virtual QString getRefSource() = 0;


    bool startDicomServer();
    bool endDicomServer();

    void loadSettings(const QSettings &) override;
    void saveSettings(QSettings*) const override;

    DicomSCP* m_dicomSCP;

    void dicomFilesReceived(QStringList paths);

protected slots:
    void dicomServerExitNormal();
    void dicomServerExitCrash();
};

#endif // DXAMANAGER_H
