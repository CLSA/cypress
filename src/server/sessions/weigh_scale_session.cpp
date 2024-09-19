#include "weigh_scale_session.h"
#include "managers/weigh_scale/weigh_scale_manager.h"
#include "dialogs/weigh_scale_dialog.h"

WeighScaleSession::WeighScaleSession(QObject *parent, const QJsonObject& inputData, const QString& origin)
    : CypressSession{parent, inputData, origin}
{
}

void WeighScaleSession::initializeDialog()
{
    m_dialog = new WeighScaleDialog(nullptr, QSharedPointer<WeighScaleSession>(this));
}

void WeighScaleSession::isInstalled() const
{
    if (WeighScaleManager::config.hasErrors())
        throw NotInstalledError("Weight scale is not installed on this workstation.");
}

void WeighScaleSession::isAvailable() const
{

}
