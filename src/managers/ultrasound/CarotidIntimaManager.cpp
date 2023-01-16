#include "CarotidIntimaManager.h"

CarotidIntimaManager::CarotidIntimaManager(QWidget *parent)
    : ManagerBase{parent}
{


}

void CarotidIntimaManager::start()
{

}

void CarotidIntimaManager::measure()
{

}

void CarotidIntimaManager::finish()
{
    emit complete(QJsonObject());
}

void CarotidIntimaManager::clearData()
{

}

void CarotidIntimaManager::loadSettings(const QSettings &)
{

}

void CarotidIntimaManager::saveSettings(QSettings*) const
{

}


QJsonObject CarotidIntimaManager::toJsonObject() const
{
    return QJsonObject {{}};
}

void CarotidIntimaManager::initializeModel()
{

}

void CarotidIntimaManager::updateModel()
{

}

void CarotidIntimaManager::setInputData(const QVariantMap &)
{

}



