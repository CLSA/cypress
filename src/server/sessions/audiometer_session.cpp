#include "audiometer_session.h"
#include "dialogs/audiometer_dialog.h"

AudiometerSession::AudiometerSession(QObject *parent, const QJsonObject& inputData)
    : CypressSession{parent, inputData}
{

}

void AudiometerSession::validate() const
{
    CypressSession::validate();
}

void AudiometerSession::calculateInputs()
{

}

void AudiometerSession::start()
{
    m_dialog.reset(new AudiometerDialog(nullptr, *this));
    if (m_dialog.isNull())
        throw QException();

    m_startDateTime = QDateTime::currentDateTimeUtc();
    m_status = SessionStatus::Started;

    m_dialog->run();
    m_dialog->show();

    qDebug() << "start session" << getSessionId() << m_startDateTime;
}

void AudiometerSession::end()
{

}
