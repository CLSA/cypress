#include "cdtt_session.h"
#include "dialogs/cdtt_dialog.h"

CDTTSession::CDTTSession(QObject *parent, const QJsonObject& inputData)
    : CypressSession{parent, inputData}
{

}

void CDTTSession::validate() const
{

}

void CDTTSession::calculateInputs()
{

}

void CDTTSession::start()
{
    m_dialog.reset(new CDTTDialog(nullptr, *this));
    if (m_dialog.isNull())
        throw QException();

    m_startDateTime = QDateTime::currentDateTimeUtc();
    m_status = SessionStatus::Started;

    m_dialog->run();
    m_dialog->show();

    qDebug() << "start session" << getSessionId() << m_startDateTime;

}

void CDTTSession::end()
{

}
