#include "ecg_session.h"
#include "managers/ecg/ecg_manager.h"
#include "dialogs/ecg_dialog.h"

ECGSession::ECGSession(QObject *parent, const QJsonObject& inputData)
    : CypressSession{parent, inputData}
{
}

void ECGSession::initializeDialog()
{
    m_dialog = new EcgDialog(nullptr, QSharedPointer<ECGSession>(this));
}

void ECGSession::isInstalled() const
{
    if (!ECGManager::isInstalled())
        throw NotInstalledError("ECG is not installed on this workstation");
}

void ECGSession::isAvailable() const
{

}


