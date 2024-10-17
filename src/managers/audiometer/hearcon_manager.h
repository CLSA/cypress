#ifndef HEARCON_MANAGER_H
#define HEARCON_MANAGER_H

#include "managers/manager_base.h"
#include "config/device_config.h"

#include <QProcess>
#include <QFile>

#include "server/sessions/hearcon_session.h"

class HearconManager : public ManagerBase
{
public:
    HearconManager(QSharedPointer<HearconSession> session);

    static DeviceConfig config;

    void addManualEntry(const QString side, const QString test, const int level, const bool pass);
    void removeMeasurement(const int index);

public slots:
    bool start();
    void measure();
    void finish();

private:
    QProcess m_hearcon;
    QProcess m_plugin;

    QFile m_backupDatabaseFile;
    QFile m_existingDatabase;
    QFile m_processFile;

    QString m_backupDatabasePath;
    QString m_existingDatabasePath;
    QString m_processPath;
    QString m_processName;
    QString m_workingPath;

    QString m_readerPath;
    QString m_readerWorkingDirectory;
    QString m_readerOutputPath;

    bool processAlreadyRunning();
    bool restoreDatabase();
    bool configurePlugin(const QString& operation);
    bool configureProcess();
};

#endif // HEARCON_MANAGER_H
