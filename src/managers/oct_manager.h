#ifndef OCT_MANAGER_H
#define OCT_MANAGER_H

#include "manager_base.h"
#include "server/sessions/oct_session.h"

#include <QProcess>

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
    QProcess m_process;
    QString m_runnablePath;
    QString m_runnableName;
    QString m_webpage;

    bool clearData() override;
    bool setUp() override;
};

#endif // OCT_MANAGER_H
