#ifndef MAC5_MANAGER_H
#define MAC5_MANAGER_H

#include "managers/manager_base.h"
#include "config/device_config.h"
#include "dicom/dicom_directory_watcher.h"

#include "server/sessions/mac5_session.h"

#include <QDomNode>

class Mac5Manager : public ManagerBase
{
public:
    Mac5Manager(QSharedPointer<Mac5Session> session);
    static DeviceConfig config;

public slots:
    bool start() override;
    void measure() override;
    void finish() override;
    void readOutput() override;

private:
    QSharedPointer<DicomDirectoryWatcher> m_directoryWatcher;

    // Directory for export
    QString m_exportPath;

    // Exported files
    QString m_xmlFilePath;
    QString m_pdfFilePath;
    QString m_rawFilePath;


    QJsonObject dfs(QDomNode node);
};

#endif // MAC5_MANAGER_H
