#include "retinal_camera_session.h"
#include "managers/retinal_camera/retinal_camera_manager.h"

#include "dialogs/retinal_camera_dialog.h"

RetinalCameraSession::RetinalCameraSession(
    QObject *parent,
    const QJsonObject &inputData,
    const QString& origin,
    Side side
)
    : CypressSession{parent, inputData, origin}
    , m_side(side)
{
    qDebug() << side;
}

void RetinalCameraSession::initializeDialog()
{
    m_dialog = new RetinalCameraDialog(nullptr, QSharedPointer<RetinalCameraSession>(this));
}


RetinalCameraSession::Side RetinalCameraSession::getSide()
{
    return m_side;
}

void RetinalCameraSession::isInstalled() const
{
    if (RetinalCameraManager::config.hasErrors())
        throw NotInstalledError("Retinal camera is not installed on this workstation");
}

void RetinalCameraSession::isAvailable() const
{

}
