#ifndef ORA_MANAGER_H
#define ORA_MANAGER_H

#include "managers/manager_base.h"
#include "server/sessions/ora_session.h"
#include "config/device_config.h"

#include <QProcess>
#include <QSqlDatabase>


class ORAManager: public ManagerBase
{
public:
    ORAManager(QSharedPointer<ORASession> session);
    static DeviceConfig config;

signals:
    void status(const QString newStatus);

public slots:
    bool start() override;
    void measure() override {};
    void finish() override;
    void readOutput() override;

private:
    QProcess m_process;
    QSqlDatabase m_database;

    QString m_runnableName;
    QString m_runnablePath;

    QString m_databasePath;
    QString m_backupDatabasePath;

    QVariantMap extractMeasures(const QString &eye);
};

#endif // ORA_MANAGER_H
