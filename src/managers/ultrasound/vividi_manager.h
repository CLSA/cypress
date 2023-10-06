#ifndef VIVIDI_MANAGER_H
#define VIVIDI_MANAGER_H

#include "managers/manager_base.h"
#include "dicom/dcm_recv.h"
#include "dicom/dicom_directory_watcher.h"
#include "data/cimt_vivid_i_test.h"

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
    void cancel();

    void addManualMeasurement() override {};

protected:
    bool setUp() override;
    bool clearData() override;
    bool cleanUp() override;

    void setInputData(const QVariantMap&) override;

private:
    CimtVividiTest* m_test;

    QScopedPointer<DcmRecv> m_dcmRecv;
    QScopedPointer<DicomDirectoryWatcher> m_dcmWatcher;
};

#endif // VIVIDI_MANAGER_H
