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
    void start() override;
    void measure() override;
    void finish() override;

    void readOutput();

    void addManualMeasurement() override;

private:
    void configureProcess();

    QProcess m_process;
    QSqlDatabase m_database;

    QString m_runnablePath;
    QString m_runnableName;

    QString m_databasePath;
    QString m_backupPath;

    // Set up device
    bool setUp() override;

    // Reset the session
    bool clearData() override;

    // Clean up the device for next time
    bool cleanUp() override;
};

#endif // GRIP_STRENGTH_MANAGER_H
