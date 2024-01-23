#include "grip_strength_session.h"
#include "managers/grip_strength/grip_strength_manager.h"

#include "dialogs/grip_strength_dialog.h"

GripStrengthSession::GripStrengthSession(QObject *parent, const QJsonObject& inputData)
    : CypressSession{parent, inputData}
{
}

void GripStrengthSession::isInstalled() const
{
    if (!GripStrengthManager::isInstalled())
        throw NotInstalledError("Grip Strength is not installed on this workstation");
}


void GripStrengthSession::isAvailable() const
{
}

void GripStrengthSession::initializeDialog()
{
    m_dialog = new GripStrengthDialog(nullptr, QSharedPointer<GripStrengthSession>(this));
}
