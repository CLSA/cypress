#ifndef DXAHIPMANAGER_H
#define DXAHIPMANAGER_H

#include "managers/manager_base.h"

#include "dicom/dicom_scp.h"
#include "dicom/dcm_recv.h"
#include "dicom/dicom_directory_watcher.h"

#include "server/sessions/dxa/dxa_hip_session.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "data/dxa/smbfilecopier.h"

#include <QMap>
#include <QObject>
#include <QString>
#include <QVariant>

/*
 * Static ivar needed for computing T- and Z-scores. Map distinct BMD variable name(s) (eg., HTOT_BMD) for a given
 * PatScanDb table (eg., Hip) and the corresponding bonerange code in the RefScanDb ReferenceCurve table (eg., 123.).
 * Additional BMD variables and codes should be added here for other tables (ie., Spine).
 */


class DxaHipManager: public ManagerBase
{
    Q_OBJECT
public:
    explicit DxaHipManager(QSharedPointer<DxaHipSession> session);
    ~DxaHipManager();

    static bool isInstalled();

    QJsonObject scanAnalysisJson;
    QJsonObject scoresJson;

    QVariantMap getParticipantData();

    QMap<QString, QVariant> retrieveDeviceData();
    QMap<QString, QVariant> extractScanAnalysisData();
    QMap<QString, QVariant> computeTandZScores();

public slots:
    void start() override;
    void measure() override;
    void finish() override;

    void dicomFilesReceived();

protected slots:
    void databaseCopied(QFileInfo fileInfo);

private:
    QString m_runnableName;
    QString m_runnablePath;
    QString m_aeTitle;
    QString m_host;
    QString m_port;
    QString m_storageDirPath;
    QString m_logConfigPath;
    QString m_ascConfigPath;

    QScopedPointer<DcmRecv> m_dicomServer;
    QScopedPointer<SMBFileCopier> m_networkFileCopier;

    QFileInfo m_patscanDbFileInfo;
    QFileInfo m_refDbFileInfo;

    // Reset the session
    bool clearData() override;

    // Set up device
    bool setUp() override;

    // Clean up the device for next time
    bool cleanUp() override;
};

#endif // DXAHIPMANAGER_H
