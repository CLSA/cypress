#include "spirometer_session.h"

#include "dialogs/spirometer_dialog.h"

SpirometerSession::SpirometerSession(QObject *parent, const QJsonObject& inputData)
    : CypressSession{parent, inputData}
{

}

void SpirometerSession::validate() const
{

}

void SpirometerSession::calculateInputs()
{

}

void SpirometerSession::start()
{
    m_dialog.reset(new SpirometerDialog(nullptr, *this));
    if (m_dialog.isNull())
        throw QException();

    m_startDateTime = QDateTime::currentDateTimeUtc();
    m_status = SessionStatus::Started;

    m_dialog->run();
    m_dialog->show();

    qDebug() << "start session" << getSessionId() << m_startDateTime;

}

void SpirometerSession::end()
{

}
