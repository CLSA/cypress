#include "cdtt_session.h"
#include "managers/cdtt/cdtt_manager.h"
#include "dialogs/cdtt_dialog.h"

CDTTSession::CDTTSession(QObject *parent, const QJsonObject& inputData)
    : CypressSession{parent, inputData}
{
}

void CDTTSession::isInstalled() const {
    if (!CDTTManager::isInstalled())
        throw NotInstalledError("CDTT is not installed on this workstation");
}

void CDTTSession::isAvailable() const
{

}

void CDTTSession::initializeDialog()
{
    m_dialog = new CDTTDialog(nullptr, QSharedPointer<CDTTSession>(this));
}
