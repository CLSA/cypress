#include "ecg_session.h"

#include "dialogs/ecg_dialog.h"

ECGSession::ECGSession(QObject *parent, const QJsonObject& inputData)
    : CypressSession{parent, inputData}
{

}

void ECGSession::start()
{
    m_dialog = new EcgDialog(nullptr, QSharedPointer<ECGSession>(this));
    if (m_dialog == nullptr)
        throw QException();

    m_startDateTime = QDateTime::currentDateTimeUtc();
    m_status = SessionStatus::Started;

    m_dialog->run();
    m_dialog->show();

    if (m_debug)
        qDebug() << "ECGSession::start " << getSessionId() << m_startDateTime;
}
