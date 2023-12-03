#include "retinal_camera_session.h"

#include "dialogs/retinal_camera_dialog.h"

RetinalCameraSession::RetinalCameraSession(QObject *parent, const QJsonObject &inputData, Side side)
    : CypressSession{parent, inputData}
    , m_side(side)
{
    qDebug() << side;
}

void RetinalCameraSession::validate() const
{
    CypressSession::validate();
}

Side RetinalCameraSession::getSide()
{
    return m_side;
}

void RetinalCameraSession::start()
{
    m_dialog = new RetinalCameraDialog(nullptr, QSharedPointer<RetinalCameraSession>(this));
    if (m_dialog == nullptr)
        throw QException();

    m_startDateTime = QDateTime::currentDateTimeUtc();
    m_status = SessionStatus::Started;

    m_dialog->run();
    m_dialog->show();

    if (m_debug)
        qDebug() << "RetinalCameraSession::start " << getSessionId() << m_startDateTime;
}
