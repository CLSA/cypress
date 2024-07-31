#ifndef EASYONE_MANAGER_H
#define EASYONE_MANAGER_H

#include "data/spirometer/tests/easyone_test.h"
#include "managers/manager_base.h"
#include "server/sessions/easyone_connect_session.h"
#include <QProcess>

class EasyoneConnectManager : public ManagerBase
{
    Q_OBJECT
public:
    EasyoneConnectManager(QSharedPointer<EasyoneConnectSession> session);
    ~EasyoneConnectManager() = default;

    static bool isInstalled();

public slots:
    bool start() override;
    void measure() override;
    void finish() override;
    void readOutput() override;

private:
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

    QProcess m_process;
    QSharedPointer<EasyoneTest> m_test;

    bool clearData() override;
    bool setUp() override;
    bool cleanUp() override;

    void configureProcess();

    QString getOutputPdfPath() const;
    bool outputPdfExists() const;
};

#endif // EASYONE_MANAGER_H
