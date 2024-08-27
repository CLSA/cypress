#include "ora_session.h"

#include "dialogs/ora_dialog.h"
#include "managers/tonometer/ora_manager.h"

ORASession::ORASession(QObject* parent, const QJsonObject& inputData, const QString& origin): CypressSession(parent, inputData, origin)
{

}

void ORASession::initializeDialog()
{
    m_dialog = new ORADialog(nullptr, QSharedPointer<ORASession>(this));
}

void ORASession::isInstalled() const
{
    if (ORAManager::config.hasErrors())
        throw NotInstalledError("ORA is not installed");
}

void ORASession::validate() const
{
    CypressSession::validate();
    if (!isValidDate("dob", "yyyy-MM-dd"))
        throw ValidationError("dob");

    if (!isValidString("sex"))
        throw ValidationError("sex");
}

void ORASession::calculateInputs()
{

}



void ORASession::isAvailable() const
{
}


