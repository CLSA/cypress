#include "tonometer_session.h"
#include "dialogs/tonometer_dialog.h"

TonometerSession::TonometerSession(QObject *parent, const QJsonObject& inputData)
    : CypressSession{parent, inputData}
{

}

void TonometerSession::validate() const
{

}

void TonometerSession::calculateInputs()
{

}

void TonometerSession::start()
{
    m_dialog.reset(new TonometerDialog(nullptr, *this));
    if (m_dialog.isNull())
        throw QException();

    m_startDateTime = QDateTime::currentDateTimeUtc();
    m_status = SessionStatus::Started;

    m_dialog->run();
    m_dialog->show();

    qDebug() << "start session" << getSessionId() << m_startDateTime;
}

void TonometerSession::end()
{

}
