#ifndef GRIPSTRENGTHMANAGER_H
#define GRIPSTRENGTHMANAGER_H

#include <QObject>
#include <QJsonDocument>
#include <QProcess>
#include <QTemporaryDir>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "managers/ManagerBase.h"
#include "data/grip_strength/tests/GripStrengthTest.h"

class GripStrengthManager : public ManagerBase
{
    Q_OBJECT

public:
    explicit GripStrengthManager(QObject *parent = nullptr);
    ~GripStrengthManager();

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

    // Send test data to Pine server
    bool sendResultsToPine(const QJsonObject& data) override;
};

#endif // GRIPSTRENGTHMANAGER_H
