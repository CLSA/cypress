#include "ora_manager.h"

ORAManager::ORAManager(QSharedPointer<ORASession> session): ManagerBase { session }
{

}

ORAManager::~ORAManager()
{

}

bool ORAManager::isInstalled()
{
    return false;
}

bool ORAManager::start()
{
    return false;
}

void ORAManager::measure()
{

}

void ORAManager::finish()
{

}

bool ORAManager::cleanUp()
{
    return true;
}

void ORAManager::readOutput()
{

}

bool ORAManager::clearData()
{
    return true;
}

bool ORAManager::setUp()
{
    return true;
}
