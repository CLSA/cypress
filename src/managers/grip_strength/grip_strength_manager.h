#ifndef GRIP_STRENGTH_MANAGER_H
#define GRIP_STRENGTH_MANAGER_H

#include <QObject>
#include <QJsonDocument>
#include <QProcess>
#include <QTemporaryDir>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "managers/manager_base.h"
#include "data/grip_strength/tests/grip_strength_test.h"

class GripStrengthManager : public ManagerBase
{
    Q_OBJECT

public:
    explicit GripStrengthManager(const CypressSession& session);
    ~GripStrengthManager();

    static bool isInstalled();
    static bool isAvailable();

public slots:
    void start() override;
    void measure() override;
    void finish() override;

private:
    GripStrengthTest m_test;
    QProcess m_process;
    QSqlDatabase m_database;

    QString m_workingDirPath;
    QString m_executablePath;
    QString m_databaseName;

    QDir m_backupDir;
    QDir m_trackerDir;

    // Set data from Pine request
    void setInputData(const QVariantMap&) override;

    // Set up device
    bool setUp() override;

    // Reset the session
    bool clearData() override;

    // Clean up the device for next time
    bool cleanUp() override;
};

#endif // GRIP_STRENGTH_MANAGER_H
