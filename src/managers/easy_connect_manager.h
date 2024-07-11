#ifndef EASY_CONNECT_MANAGER_H
#define EASY_CONNECT_MANAGER_H

#include "manager_base.h"

class EasyConnectManager : public ManagerBase
{
public:
    EasyConnectManager(QSharedPointer<CypressSession> session);

    // ManagerBase interface
public slots:
    bool start() override;
    void measure() override;
    void finish() override;
    void readOutput() override;
    void addManualMeasurement() override;
    bool clearData() override;
    bool cleanUp() override;

protected:
    bool setUp() override;
};

#endif // EASY_CONNECT_MANAGER_H
