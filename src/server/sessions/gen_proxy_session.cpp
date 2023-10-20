#include "gen_proxy_session.h"

GenProxySession::GenProxySession(QObject *parent, const QJsonObject& inputData)
    : CypressSession{parent, inputData}
{

}

void GenProxySession::validate() const
{
    CypressSession::validate();
}

void GenProxySession::calculateInputs()
{

}

void GenProxySession::start()
{
    m_startDateTime = QDateTime::currentDateTimeUtc();
    m_status = SessionStatus::Started;

    m_dialog->run();
    m_dialog->show();

    qDebug() << "start session" << getSessionId() << m_startDateTime;

}

void GenProxySession::end()
{

}
