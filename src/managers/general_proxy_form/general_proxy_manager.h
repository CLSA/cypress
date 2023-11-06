#ifndef GENERALPROXYMANAGER_H
#define GENERALPROXYMANAGER_H

#include "../manager_base.h"
#include "server/sessions/gen_proxy_session.h"

class GeneralProxyManager : public ManagerBase
{
public:
    GeneralProxyManager(QSharedPointer<GenProxySession> session);

    // ManagerBase interface
public slots:
    void start();
    void measure();
    void finish();

protected:
    void setInputData(const QVariantMap &);
    bool setUp();
    bool clearData();
    bool cleanUp();
};

#endif // GENERALPROXYMANAGER_H
