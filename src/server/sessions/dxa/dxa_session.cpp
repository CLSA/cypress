#include "dxa_session.h"

#include "dialogs/dxa_dialog.h"

DXASession::DXASession(QObject *parent, const QJsonObject& inputData)
    : CypressSession{parent, inputData}
{

}

void DXASession::start()
{
    m_dialog = new DXADialog(nullptr, QSharedPointer<DXASession>(this));
    if (m_dialog == nullptr)
        throw QException();

    m_startDateTime = QDateTime::currentDateTimeUtc();
    m_status = SessionStatus::Started;

    m_dialog->run();
    m_dialog->show();

    if (m_debug)
        qDebug() << "DXASession::start " << getSessionId() << m_startDateTime;
}


