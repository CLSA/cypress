#ifndef DXA_MANAGER_H
#define DXA_MANAGER_H

#include "managers/manager_base.h"
#include "server/sessions/dxa/dxa_session.h"

#include "dicom/dcm_recv.h"

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
    static QString getWebpageContents(const DXASession &session);

public slots:
    bool start() override;
    void measure() override;
    void finish() override;

private slots:
    void dicomFilesReceived(QList<DicomFile> files);

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

    // Reset the session
    bool clearData() override;

    // Set up device
    bool setUp() override;

    // Clean up the device for next time
    bool cleanUp() override;
};

#endif // DXA_MANAGER_H
