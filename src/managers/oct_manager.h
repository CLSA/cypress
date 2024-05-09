#ifndef OCT_MANAGER_H
#define OCT_MANAGER_H

#include "manager_base.h"

#include "server/sessions/oct_session.h"

#include "dicom/dcm_recv.h"

class OCTManager : public ManagerBase
{
public:
    OCTManager(QSharedPointer<OCTSession> session);
    ~OCTManager();

    static bool isInstalled();

signals:
    void status(const QString newStatus);

public slots:
    bool start() override;
    void measure() override;
    void finish() override;
    bool cleanUp() override;

private slots:
    void dicomFilesReceived(QList<DicomFile> dicomFiles);

private:
    QString m_runnablePath;
    QString m_runnableName;
    QString m_aeTitle;
    QString m_host;
    QString m_port;
    QString m_storageDirPath;
    QString m_logConfigPath;
    QString m_ascConfigPath;

    QScopedPointer<DcmRecv> m_dicomServer;

    bool clearData() override;
    bool setUp() override;
};

#endif // OCT_MANAGER_H
