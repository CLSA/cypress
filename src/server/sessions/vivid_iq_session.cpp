#include "vivid_iq_session.h"

#include "dialogs/vivid_iq_dialog.h"
#include "managers/ultrasound/vivid_iq_manager.h"

#include "auxiliary/json_settings.h"

VividIQSession::VividIQSession(QObject *parent, const QJsonObject &inputData, const QString &origin)
    : CypressSession(parent, inputData, origin)
{
    qDebug().noquote() << JsonSettings::prettyPrintJson(inputData);
}

void VividIQSession::initializeDialog()
{
    m_dialog = new VividIQDialog(nullptr, QSharedPointer<VividIQSession>(this));
}

void VividIQSession::isInstalled() const
{
    if (VividIQManager::config.hasErrors())
        throw NotInstalledError("VividIQ is not installed");
}

void VividIQSession::isAvailable() const
{

}
