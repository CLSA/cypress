#include "retinal_camera_session.h"

#include "dialogs/retinal_camera_dialog.h"

RetinalCameraSession::RetinalCameraSession(QObject *parent, const QJsonObject& inputData)
    : CypressSession{parent, inputData}
{

}

void RetinalCameraSession::validate() const
{

}

void RetinalCameraSession::calculateInputs()
{

}

void RetinalCameraSession::start()
{
    m_dialog.reset(new RetinalCameraDialog(nullptr, *this));
    if (m_dialog.isNull())
        throw QException();

    m_startDateTime = QDateTime::currentDateTimeUtc();
    m_status = SessionStatus::Started;

    m_dialog->run();
    m_dialog->show();

    qDebug() << "start session" << getSessionId() << m_startDateTime;

}

void RetinalCameraSession::end()
{

}
