#ifndef VIVID_IQ_MANAGER_H
#define VIVID_IQ_MANAGER_H

#include "managers/manager_base.h"
#include "config/device_config.h"
#include "dicom/dcm_recv.h"
#include "cypress_session.h"

class VividIQManager : public ManagerBase
{
    Q_OBJECT
public:
    VividIQManager(QSharedPointer<CypressSession> session);

    static DeviceConfig config;

signals:
    void filesReceived(int numFiles, QString totalSize);

    // ManagerBase interface
public slots:
    bool start();
    void measure() {};
    void finish();

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
};

#endif // VIVID_IQ_MANAGER_H
