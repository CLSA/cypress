#ifndef DXA_MANAGER_H
#define DXA_MANAGER_H

#include "../../data/dxa/smbfilecopier.h"
#include "../manager_base.h"
#include "./dicom/dicom_scp.h"
#include "server/sessions/dxa/dxa_session.h"

#include "dcmtk/dcmdata/dcfilefo.h"
#include "dicom/dcm_recv.h"
#include "dicom/dicom_directory_watcher.h"

#include <QMap>
#include <QObject>
#include <QString>
#include <QVariant>

/*
 * Static ivar needed for computing T- and Z-scores. Map distinct BMD variable name(s) (eg., HTOT_BMD) for a given
 * PatScanDb table (eg., Hip) and the corresponding bonerange code in the RefScanDb ReferenceCurve table (eg., 123.).
 * Additional BMD variables and codes should be added here for other tables (ie., Spine).
 */


class DXAManager : public ManagerBase
{
    Q_OBJECT
public:
    explicit DXAManager(QSharedPointer<DXASession> session);
    ~DXAManager();

    static bool isInstalled();

    QJsonObject scanAnalysisJson;
    QJsonObject scoresJson;

    QList<DcmFileFormat> validatedDicomFiles;
    QList<DcmFileFormat> getValidatedFiles(QStringList filePaths);

    QVariantMap getParticipantData();

    QMap<QString, QVariant> retrieveDeviceData();
    QMap<QString, QVariant> extractScanAnalysisData();
    QMap<QString, QVariant> computeTandZScores();

    bool validateDicomFile(DcmFileFormat &loadedFileFormat);

    bool isCorrectDicom(DcmFileFormat &file);
    bool isCompleteDicom(DcmFileFormat &file);

public slots:
    void start() override;
    void measure() override;
    void finish() override;

    void dicomFilesReceived();
    void copiedDatabaseFile(QFileInfo file);

protected slots:
    void dicomServerExitNormal();
    void dicomServerExitCrash();

signals:
    void updateDicomTable(QList<DicomFile> files);

private:
    QFileInfo m_patscanDb{};
    QFileInfo m_referenceDb{};

    QScopedPointer<DcmRecv> m_dicomServer;
    QScopedPointer<SMBFileCopier> m_networkFileCopier;

    // Reset the session
    bool clearData() override;

    // Set up device
    bool setUp() override;

    // Clean up the device for next time
    bool cleanUp() override;
};

#endif // DXA_MANAGER_H
