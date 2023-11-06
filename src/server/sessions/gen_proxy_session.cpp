#include "gen_proxy_session.h"

#include "managers/general_proxy_form/general_proxy_manager.h"

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

    GeneralProxyManager manager(QSharedPointer<GenProxySession>(this));
    manager.start();

    qDebug() << "start session" << getSessionId() << m_startDateTime;
}

void GenProxySession::end()
{

}
