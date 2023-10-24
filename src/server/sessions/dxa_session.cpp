#include "dxa_session.h"

#include "dialogs/dxa_dialog.h"

DXASession::DXASession(QObject *parent, const QJsonObject& inputData)
    : CypressSession{parent, inputData}
{

}

void DXASession::validate() const
{
    CypressSession::validate();
}

void DXASession::calculateInputs()
{

}

void DXASession::start()
{
    m_dialog.reset(new DXADialog(nullptr, *this));
    if (m_dialog.isNull())
        throw QException();

    m_startDateTime = QDateTime::currentDateTimeUtc();
    m_status = SessionStatus::Started;

    m_dialog->run();
    m_dialog->show();

    qDebug() << "start session" << getSessionId() << m_startDateTime;

}

void DXASession::end()
{

}
