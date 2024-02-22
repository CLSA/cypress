#ifndef DXAHIPMANAGER_H
#define DXAHIPMANAGER_H

#include "managers/manager_base.h"

#include "dicom/dcm_recv.h"

#include "server/sessions/dxa/dxa_hip_session.h"

#include <QMap>
#include <QObject>
#include <QSqlDatabase>
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
    static QString getWebpageContents(const DxaHipSession &session);

public slots:
    bool start() override;
    void measure() override;
    void finish() override;

private slots:
    void dicomFilesReceived(QList<DicomFile> dicomFiles);

private:
    QScopedPointer<DcmRecv> m_dicomServer;

    QString m_runnableName;
    QString m_runnablePath;

    QString m_patscanDbPath;
    QString m_refscanDbPath;

    QString m_storageDirPath;
    QString m_aeTitle;
    QString m_host;
    QString m_port;
    QString m_logConfigPath;
    QString m_ascConfigPath;

    QFileInfo m_patscanDbFileInfo;
    QSqlDatabase m_patscanDb;

    QFileInfo m_referenceDbFileInfo;
    QSqlDatabase m_referenceDb;

    // Reset the session
    bool clearData() override;

    // Set up device
    bool setUp() override;

    // Clean up the device for next time
    bool cleanUp() override;

    bool initReferenceDb();
    bool initPatScanDb();

    bool copyReferenceDb();
    bool copyPatScanDb();
};

#endif // DXAHIPMANAGER_H
