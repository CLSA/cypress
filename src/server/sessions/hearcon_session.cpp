#include "hearcon_session.h"

#include "dialogs/hearcon_dialog.h"
#include "managers/hearcon_manager.h"

HearconSession::HearconSession(QObject *parent, const QJsonObject& inputData, const QString& origin)
    : CypressSession{parent, inputData, origin} {
}

void HearconSession::isInstalled() const {
    if (HearconManager::config.hasErrors())
        throw NotInstalledError("Audiometer is not installed on this workstation");
}

void HearconSession::isAvailable() const {

}

void HearconSession::validate() const {
    CypressSession::validate();

    if (!isValidString("sex"))
        throw ValidationError("sex");

    if (!isValidDate("dob", "yyyy-MM-dd"))
        throw ValidationError("dob");
}

void HearconSession::initializeDialog() {
    m_dialog = new HearconDialog(nullptr, QSharedPointer<HearconSession>(this));
}
