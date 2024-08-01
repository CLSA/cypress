#include "watch_bp_session.h"

#include "managers/watch_bp_manager.h"
#include "dialogs/watch_bp_dialog.h"

WatchBPSession::WatchBPSession(QObject *parent, const QJsonObject& inputData, const QString& origin)
    : CypressSession{parent, inputData, origin}
{

}

void WatchBPSession::isInstalled() const
{
    if (WatchBPManager::config.hasErrors())
        throw NotInstalledError("");
}

void WatchBPSession::isAvailable() const
{

}

void WatchBPSession::initializeDialog()
{
    m_dialog = new WatchBPDialog(nullptr, QSharedPointer<WatchBPSession>(this));
}

void WatchBPSession::validate() const
{

}

void WatchBPSession::calculateInputs()
{

}
