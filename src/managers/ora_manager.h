#ifndef ORA_MANAGER_H
#define ORA_MANAGER_H

#include "manager_base.h"
#include "server/sessions/ora_session.h"


#include <QProcess>
#include <QSqlDatabase>


class ORAManager: public ManagerBase
{
public:
    ORAManager(QSharedPointer<ORASession> session);
    ~ORAManager();

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

#endif // ORA_MANAGER_H
