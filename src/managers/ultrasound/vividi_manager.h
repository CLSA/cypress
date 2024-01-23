#ifndef VIVIDI_MANAGER_H
#define VIVIDI_MANAGER_H

#include "managers/manager_base.h"
#include "dicom/dcm_recv.h"

#include "server/sessions/ultrasound_session.h"

#include <QObject>

class VividiManager: public ManagerBase
{
    Q_OBJECT
public:
    explicit VividiManager(QSharedPointer<UltrasoundSession> session);

    ~VividiManager();

    static bool isInstalled();

public slots:
    bool start() override;
    void measure() override;
    void finish() override;

private slots:
    void dicomFilesReceived(QList<DicomFile> dicomFiles);

protected:
    bool setUp() override;
    bool clearData() override;
    bool cleanUp() override;

private:
    QString m_runnablePath;
    QString m_runnableName;
    QString m_aeTitle;
    QString m_host;
    QString m_port;
    QString m_storageDirPath;
    QString m_logConfigPath;
    QString m_ascConfigPath;

    int m_stillImageIdCounter = 1;
    int m_cineloopCounter = 1;
    int m_srCounter = 1;

    QScopedPointer<DcmRecv> m_dicomServer;
};

#endif // VIVIDI_MANAGER_H
