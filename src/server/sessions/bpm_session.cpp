#include "bpm_session.h"
#include "managers/blood_pressure/blood_pressure_manager.h"
#include "dialogs/blood_pressure_dialog.h"

BPMSession::BPMSession(QObject *parent, const QJsonObject& inputData)
    : CypressSession{parent, inputData}
{
}

void BPMSession::isInstalled() const {
    if (!BloodPressureManager::isInstalled())
        throw NotInstalledError("");
}

void BPMSession::isAvailable() const
{

}


void BPMSession::initializeDialog()
{
    m_dialog = new BloodPressureDialog(nullptr, QSharedPointer<BPMSession>(this));
}
