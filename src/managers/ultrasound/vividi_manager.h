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

    static bool isInstalled();

public slots:
    void start() override;
    void measure() override;
    void finish() override;
    void cancel();

    void addManualMeasurement() override {};

    void dicomFilesReceived();

protected:
    bool setUp() override;
    bool clearData() override;
    bool cleanUp() override;

private:
    QScopedPointer<DcmRecv> m_dicomServer;
};

#endif // VIVIDI_MANAGER_H
