#ifndef GENERALPROXYMANAGER_H
#define GENERALPROXYMANAGER_H

#include "../manager_base.h"
#include "server/sessions/gen_proxy_session.h"

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

private:
    QString m_inputFilePath;
    QString m_outputFilePath;
};

#endif // GENERALPROXYMANAGER_H
