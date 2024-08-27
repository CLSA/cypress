#ifndef OCT_MANAGER_H
#define OCT_MANAGER_H

#include "managers/manager_base.h"
#include "config/device_config.h"

#include "server/sessions/oct_session.h"
#include "dicom/dicom_directory_watcher.h"

#include <QProcess>
#include <QSqlDatabase>


class OCTManager : public ManagerBase
{

public:
    OCTManager(QSharedPointer<OCTSession> session);
    ~OCTManager();

    static DeviceConfig config;

signals:
    void status(const QString newStatus);

public slots:
    bool start() override;
    void readOutput() override;
    void finish() override;

    void measure() override {};
    bool cleanUp() override { return true; };

private:
    QString defaultPersonUUID = "11111111-2222-3333-4444-555555555555";
    QString defaultPatientUUID = "11111111-2222-3333-4444-555555555555";

    QSharedPointer<DicomDirectoryWatcher> m_directoryWatcher;

    QProcess m_process;
    QSqlDatabase m_db;

    QString m_runnablePath;
    QString m_runnableName;
    QString m_exportPath;

    QString m_databaseBackup;
    QString m_databaseName;

    void configureProcess();

    bool clearData() override { return true; };
    bool setUp() override { return true; };
};

#endif // OCT_MANAGER_H
