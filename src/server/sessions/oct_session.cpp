#include "oct_session.h"

#include "cypress_session.h"
#include "managers/retinal_camera/oct_manager.h"
#include "dialogs/oct_dialog.h"

OCTSession::OCTSession(QObject *parent, const QJsonObject& inputData, const QString& origin, OCTSession::Side side)
    : CypressSession{parent, inputData, origin}, m_side(side)
{
}

void OCTSession::initializeDialog()
{
    m_dialog = new OCTDialog(nullptr, QSharedPointer<OCTSession>(this));
}

OCTSession::Side OCTSession::getSide()
{
    return m_side;
}

void OCTSession::isInstalled() const
{
    if (OCTManager::config.hasErrors())
        throw NotInstalledError("OCT is not installed");
}

void OCTSession::isAvailable() const
{

}
