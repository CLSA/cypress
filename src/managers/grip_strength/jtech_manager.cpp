#include "jtech_manager.h"

#include "data/grip_strength/tests/jtech_test.h"

JTechManager::JTechManager(QSharedPointer<JTechSession> session) : ManagerBase(session)
{
    m_test.reset(new JTechTest);
}

bool JTechManager::start()
{
    qDebug() << "JTechManager::start";
    return true;
}

void JTechManager::readOutput()
{
    qDebug() << "JTechManager::readOutput";
    finish();
}

void JTechManager::finish()
{
    qDebug() << "JTechManager::finish";
    ManagerBase::finish();
}




