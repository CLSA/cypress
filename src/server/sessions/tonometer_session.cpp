#include "server/sessions/tonometer_session.h"
#include "managers/tonometer/tonometer_manager.h"
#include "dialogs/tonometer_dialog.h"

TonometerSession::TonometerSession(QObject *parent, const QJsonObject& inputData, const QString& origin)
    : CypressSession{parent, inputData, origin}
{
}

void TonometerSession::initializeDialog()
{
    m_dialog = new TonometerDialog(nullptr, QSharedPointer<TonometerSession>(this));
}

void TonometerSession::isInstalled() const {
    if (!TonometerManager::isInstalled())
        throw NotInstalledError("Tonometer is not installed on this workstation");
}

void TonometerSession::isAvailable() const {

}

void TonometerSession::validate() const
{
    CypressSession::validate();
    if (!isValidDate("dob", "yyyy-MM-dd"))
        throw ValidationError("dob");

    if (!isValidString("sex"))
        throw ValidationError("sex");
}
