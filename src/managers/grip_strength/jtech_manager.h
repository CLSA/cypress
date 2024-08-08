#ifndef JTECH_MANAGER_H
#define JTECH_MANAGER_H

#include "managers/manager_base.h"
#include "server/sessions/jtech_session.h"

class JTechManager : public ManagerBase
{
public:
    JTechManager(QSharedPointer<JTechSession> session);

    // ManagerBase interface
public slots:
    bool start();
    void measure() {};
    void finish();
    void readOutput();
};

#endif // JTECH_MANAGER_H
