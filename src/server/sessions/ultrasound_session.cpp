#include "ultrasound_session.h"
#include "managers/ultrasound/vividi_manager.h"
#include "dialogs/cimt_vivid_i_dialog.h"

UltrasoundSession::UltrasoundSession(QObject *parent, const QJsonObject& inputData, const QString& origin)
    : CypressSession{parent, inputData, origin}
{
}

void UltrasoundSession::initializeDialog()
{
    m_dialog = new CimtVividiDialog(nullptr, QSharedPointer<UltrasoundSession>(this));
}

void UltrasoundSession::isInstalled() const
{
    if (!VividiManager::isInstalled())
        throw NotInstalledError("Ultrasound is not installed on this workstation");
}

void UltrasoundSession::isAvailable() const
{

}
