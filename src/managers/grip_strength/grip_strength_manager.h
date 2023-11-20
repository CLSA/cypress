#ifndef GRIP_STRENGTH_MANAGER_H
#define GRIP_STRENGTH_MANAGER_H

#include "data/grip_strength/tests/grip_strength_test.h"
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
    ~GripStrengthManager();

    static bool isInstalled();

public slots:
    void start() override;
    void measure() override;
    void finish() override;

    void addManualMeasurement() override;

private:
    QProcess m_process;
    QSqlDatabase m_database;

    QString m_workingDirPath;
    QString m_executablePath;
    QString m_databaseName;

    QDir m_backupDir;
    QDir m_trackerDir;

    // Set up device
    bool setUp() override;

    // Reset the session
    bool clearData() override;

    // Clean up the device for next time
    bool cleanUp() override;
};

#endif // GRIP_STRENGTH_MANAGER_H
