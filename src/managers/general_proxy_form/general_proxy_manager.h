#ifndef GENERALPROXYMANAGER_H
#define GENERALPROXYMANAGER_H

#include "managers/manager_base.h"
#include "server/sessions/gen_proxy_session.h"

#include <QProcess>

class GeneralProxyManager : public ManagerBase
{
public:
    GeneralProxyManager(QSharedPointer<GenProxySession> session);

    static bool isInstalled();

public slots:
    bool start();
    void measure();
    void finish();

protected:
    void setInputData(const QVariantMap &);
    bool setUp();
    bool clearData();
    bool cleanUp();

    void readOutput();

private:
    QProcess m_process;
    QString m_inputFilePath;
    QString m_outputFilePath;

    QString m_runnableName;
    QString m_runnablePath;
    QString m_pdftkPath;
};

#endif // GENERALPROXYMANAGER_H
