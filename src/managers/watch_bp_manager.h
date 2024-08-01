#ifndef WATCH_BP_MANAGER_H
#define WATCH_BP_MANAGER_H

#include "manager_base.h"
#include "config/device_config.h"

#include "server/sessions/watch_bp_session.h"

#include <QProcess>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class WatchBPManager : public ManagerBase
{
public:
    WatchBPManager(QSharedPointer<WatchBPSession> session);

    static DeviceConfig config;

    // ManagerBase interface
public slots:
    bool start() override;
    void measure() override;
    void finish() override;
    void readOutput() override;
    void addManualMeasurement() override;
    bool clearData() override;
    bool cleanUp() override;

protected:
    bool setUp() override;

private:
    QSqlDatabase m_database;
    QProcess m_process;

    QString m_processName;
    QString m_runnableName;
    QString m_runnablePath;
    QString m_databasePath;
    QString m_backupDatabasePath;
};

#endif // WATCH_BP_MANAGER_H
