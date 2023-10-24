#include "ultrasound_session.h"

#include "dialogs/cimt_vivid_i_dialog.h"

UltrasoundSession::UltrasoundSession(QObject *parent, const QJsonObject& inputData)
    : CypressSession{parent, inputData}
{

}

void UltrasoundSession::validate() const
{
    CypressSession::validate();
}

void UltrasoundSession::calculateInputs()
{

}

void UltrasoundSession::start()
{
    m_dialog.reset(new CimtVividiDialog(nullptr, QSharedPointer<UltrasoundSession>(this)));
    if (m_dialog.isNull())
        throw QException();

    m_startDateTime = QDateTime::currentDateTimeUtc();
    m_status = SessionStatus::Started;

    m_dialog->run();
    m_dialog->show();

    qDebug() << "start session" << getSessionId() << m_startDateTime;
}

void UltrasoundSession::end()
{

}
