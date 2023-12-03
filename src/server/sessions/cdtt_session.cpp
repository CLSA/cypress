#include "cdtt_session.h"
#include "dialogs/cdtt_dialog.h"

CDTTSession::CDTTSession(QObject *parent, const QJsonObject& inputData)
    : CypressSession{parent, inputData}
{

}

void CDTTSession::start()
{
    m_dialog = new CDTTDialog(nullptr, QSharedPointer<CDTTSession>(this));
    if (m_dialog == nullptr)
        throw QException();

    m_startDateTime = QDateTime::currentDateTimeUtc();
    m_status = SessionStatus::Started;

    m_dialog->run();
    m_dialog->show();

    if (m_debug)
        qDebug() << "CDTTSession::start " << getSessionId() << m_startDateTime;
}
