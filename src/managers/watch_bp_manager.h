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

    void addManualEntry(const int systolic, const int diastolic, const int pulse);
    void removeMeasurement(const int index);

    // ManagerBase interface
public slots:
    bool start() override;
    void measure() override;
    void finish() override;

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
