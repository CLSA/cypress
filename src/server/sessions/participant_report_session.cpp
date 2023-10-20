#include "participant_report_session.h"

ParticipantReportSession::ParticipantReportSession(QObject *parent, const QJsonObject& inputData)
    : CypressSession{parent, inputData}
{

}

void ParticipantReportSession::validate() const
{
    CypressSession::validate();
}

void ParticipantReportSession::calculateInputs()
{

}

void ParticipantReportSession::start()
{
    //m_dialog.reset(new FraxDialog(nullptr, *this));
    //if (m_dialog.isNull())
    //    throw QException();

    m_startDateTime = QDateTime::currentDateTimeUtc();
    m_status = SessionStatus::Started;

    m_dialog->run();
    m_dialog->show();

    qDebug() << "start session" << getSessionId() << m_startDateTime;

}

void ParticipantReportSession::end()
{

}
