#ifndef VIVIDI_MANAGER_H
#define VIVIDI_MANAGER_H

#include "managers/manager_base.h"
#include "dicom/dcm_recv.h"
#include "dicom/dicom_directory_watcher.h"

#include <QObject>

class VividiManager: public ManagerBase
{
    Q_OBJECT
public:
    explicit VividiManager(const CypressSession& session);

    static bool isAvailable();
    static bool isInstalled();

public slots:
    void start() override;
    void measure() override;
    void finish() override;

protected:
    void setInputData(const QVariantMap&) override;
    bool setUp() override;
    bool clearData() override;
    bool cleanUp() override;

private:
    QScopedPointer<DcmRecv> m_dcmRecv;
    QScopedPointer<DicomDirectoryWatcher> m_dcmWatcher;
};

#endif // VIVIDI_MANAGER_H
