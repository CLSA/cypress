#include "cdtt_session.h"
#include "managers/cdtt/cdtt_manager.h"
#include "dialogs/cdtt_dialog.h"

#include "auxiliary/json_settings.h"

CDTTSession::CDTTSession(QObject *parent, const QJsonObject& inputData, const QString& origin)
    : CypressSession{parent, inputData, origin}
{
    qDebug().noquote() << JsonSettings::prettyPrintJson(inputData);
}

void CDTTSession::isInstalled() const {
    if (CDTTManager::config.hasErrors())
        throw NotInstalledError("CDTT is not installed on this workstation");
}

void CDTTSession::isAvailable() const
{

}

void CDTTSession::initializeDialog()
{
    m_dialog = new CDTTDialog(nullptr, QSharedPointer<CDTTSession>(this));
}
