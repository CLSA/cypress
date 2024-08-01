#ifndef HEARCON_MANAGER_H
#define HEARCON_MANAGER_H

#include "manager_base.h"
#include "config/device_config.h"

#include <QProcess>
#include <QFile>

#include "server/sessions/hearcon_session.h"

class HearconManager : public ManagerBase
{
public:
    HearconManager(QSharedPointer<HearconSession> session);

    static DeviceConfig config;

    // ManagerBase interface
public slots:
    bool start();
    void measure();
    void finish();
    void readOutput();
    void addManualMeasurement();
    bool clearData();
    bool cleanUp();

protected:
    bool setUp();

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
};

#endif // HEARCON_MANAGER_H
