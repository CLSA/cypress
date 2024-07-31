#include "ora_manager.h"

#include <QSqlQuery>
#include <QSqlError>


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
    QSqlQuery query;

    query.prepare("SELECT * FROM ");
    if (!query.exec()) {
        qCritical() << query.lastError();
        return;
    }
    if (!query.size()) {
        return;
    }

    query.prepare("SELECT * FROM ");
    if (!query.exec()) {
        qCritical() << query.lastError();
        return;
    }
    if (!query.size()) {
        return;
    }

    query.prepare("SELECT * FROM ");
    if (!query.exec()) {
        qCritical() << query.lastError();
        return;
    }

    if (!query.size()) {
        return;
    }
}

void ORAManager::finish()
{
    // send data
}

bool ORAManager::cleanUp()
{
    // restore database backup
    QProcess process;

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
