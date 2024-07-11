#ifndef HEARCON_MANAGER_H
#define HEARCON_MANAGER_H

#include "manager_base.h"

class HearconManager : public ManagerBase
{
public:
    HearconManager(QSharedPointer<CypressSession> session);
};

#endif // HEARCON_MANAGER_H
