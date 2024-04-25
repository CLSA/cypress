#ifndef GRIP_STRENGTH_MANAGER_H
#define GRIP_STRENGTH_MANAGER_H

#include "managers/manager_base.h"
#include "server/sessions/grip_strength_session.h"

#include <QJsonDocument>
#include <QObject>
#include <QProcess>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTemporaryDir>

class GripStrengthManager : public ManagerBase
{
    Q_OBJECT

public:
    explicit GripStrengthManager(QSharedPointer<GripStrengthSession> session);

    static bool isInstalled();

public slots:
    bool start() override;
    void measure() override;
    void readOutput() override;

private:
    void configureProcess();

    QProcess m_process;
    QSqlDatabase m_database;

    QString m_runnablePath;
    QString m_runnableName;

    QString m_gripTestDBPath;
    QString m_gripTestDataDBPath;

    QString m_backupPath;
    QString m_databasePath;

    // Set up device
    bool setUp() override;
    bool backupData();

    // Clean up the device for next time
    bool cleanUp() override;
    bool restoreData();

    // Reset the session
    bool clearData() override;
};

#endif // GRIP_STRENGTH_MANAGER_H
