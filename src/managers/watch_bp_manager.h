#ifndef WATCH_BP_MANAGER_H
#define WATCH_BP_MANAGER_H

#include "manager_base.h"

#include <QProcess>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class WatchBPManager : public ManagerBase
{
public:
    WatchBPManager(QSharedPointer<CypressSession> session);

    static bool isInstalled();

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

    QString m_runnableName;
    QString m_runnablePath;
    QString m_databasePath;
};

#endif // WATCH_BP_MANAGER_H
