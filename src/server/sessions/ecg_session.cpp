#include "ecg_session.h"

#include "dialogs/ecg_dialog.h"

ECGSession::ECGSession(QObject *parent, const QJsonObject& inputData)
    : CypressSession{parent, inputData}
{

}

void ECGSession::validate() const
{

}

void ECGSession::calculateInputs()
{

}

void ECGSession::start()
{
    m_dialog.reset(new EcgDialog(nullptr, *this));
    if (m_dialog.isNull())
        throw QException();

    m_startDateTime = QDateTime::currentDateTimeUtc();
    m_status = SessionStatus::Started;

    m_dialog->run();
    m_dialog->show();

    qDebug() << "start session" << getSessionId() << m_startDateTime;
}

void ECGSession::end()
{

}
