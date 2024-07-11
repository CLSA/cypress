#include "gen_proxy_session.h"
#include "dialogs/general_proxy_form_dialog.h"

#include "managers/general_proxy_form/general_proxy_manager.h"

GenProxySession::GenProxySession(QObject *parent, const QJsonObject& inputData, const QString& origin)
    : CypressSession{parent, inputData, origin}
{

}

void GenProxySession::initializeDialog() {
    m_dialog = new GeneralProxyFormDialog(nullptr, QSharedPointer<GenProxySession>(this));
}

void GenProxySession::validate() const
{
    CypressSession::validate();
}

void GenProxySession::isInstalled() const {
    if (!GeneralProxyManager::isInstalled())
        throw NotInstalledError("Not installed");
}

void GenProxySession::isAvailable() const {

}
