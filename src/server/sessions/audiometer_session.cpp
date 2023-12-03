#include "audiometer_session.h"
#include "dialogs/audiometer_dialog.h"

AudiometerSession::AudiometerSession(QObject *parent, const QJsonObject& inputData)
    : CypressSession{parent, inputData}
{

}

void AudiometerSession::start()
{
    m_dialog = new AudiometerDialog(nullptr, QSharedPointer<AudiometerSession>(this));
    if (m_dialog == nullptr)
        throw QException();

    m_startDateTime = QDateTime::currentDateTimeUtc();
    m_status = SessionStatus::Started;

    m_dialog->run();
    m_dialog->show();

    if (m_debug)
        qDebug() << "AudiometerSession::start " << getSessionId() << m_startDateTime;
}
