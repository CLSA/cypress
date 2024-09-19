#ifndef EASYONE_MANAGER_H
#define EASYONE_MANAGER_H

#include "managers/manager_base.h"
#include "config/device_config.h"

#include "data/spirometer/tests/spirometer_test.h"
#include "server/sessions/easyone_connect_session.h"
#include <QProcess>

class EasyoneConnectManager : public ManagerBase
{
    Q_OBJECT
public:
    EasyoneConnectManager(QSharedPointer<EasyoneConnectSession> session);
    ~EasyoneConnectManager() = default;

    static DeviceConfig config;

public slots:
    bool start() override;
    void measure() override {};
    void finish() override;
    void readOutput() override;

private:
    QProcess m_process;

    QString m_processName;  // Name of process when running in Task Manager
    QString m_runnableName; // Full path to EasyConnect.exe
    QString m_runnablePath; // Path to EasyConnect.exe directory
    QString m_databasePath; // Path to the database and options EasyConnect uses
    QString m_backupPath;
    QString m_exchangePath; // Path to the EMR plugin data transfer directory

    QString m_inFileName;   // CypressIn.xml
    QString m_outFileName;  // CypressOut.xml

    QString getEMRInXmlName() const;
    QString getEMROutXmlName() const;
    QString getOutputPdfPath() const;

    bool clearData() override { return true; } ;
    bool setUp() override { return true; };
    bool cleanUp() override { return true; };

    bool restoreDatabase();
    bool writeEMRRequest();
    bool configureProcess();
};

#endif // EASYONE_MANAGER_H
