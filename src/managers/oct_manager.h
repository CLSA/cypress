#ifndef OCT_MANAGER_H
#define OCT_MANAGER_H

#include "manager_base.h"
#include "server/sessions/oct_session.h"

#include <QProcess>
#include <QSqlDatabase>

class OCTManager : public ManagerBase
{
public:
    OCTManager(QSharedPointer<OCTSession> session);
    ~OCTManager();

    static bool isInstalled();

signals:
    void status(const QString newStatus);

public slots:
    bool start() override;
    void measure() override;
    void finish() override;
    bool cleanUp() override;

    void readOutput() override;

private:
    QString defaultPersonUUID = "11111111-2222-3333-4444-555555555555";
    QString defaultPatientUUID = "11111111-2222-3333-4444-555555555555";

    QProcess m_process;
    QSqlDatabase m_db;

    QString m_runnablePath;
    QString m_runnableName;
    QString m_webpage;

    QString m_databaseName;
    QString m_databasePort;
    QString m_databaseUser;

    bool clearData() override;
    bool setUp() override;
};

#endif // OCT_MANAGER_H
