#include "audiometer_session.h"
#include "dialogs/audiometer_dialog.h"
#include "managers/audiometer/audiometer_manager.h"

AudiometerSession::AudiometerSession(QObject *parent, const QJsonObject& inputData)
    : CypressSession{parent, inputData}
{
}

void AudiometerSession::isInstalled() const
{
    if (!AudiometerManager::isInstalled())
        throw NotInstalledError("Audiometer is not installed on this workstation");
}

void AudiometerSession::isAvailable() const
{

}

void AudiometerSession::initializeDialog()
{
    m_dialog = new AudiometerDialog(nullptr, QSharedPointer<AudiometerSession>(this));
}
