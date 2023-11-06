#include "tonometer_session.h"
#include "dialogs/tonometer_dialog.h"

TonometerSession::TonometerSession(QObject *parent, const QJsonObject& inputData)
    : CypressSession{parent, inputData}
{

}

void TonometerSession::validate() const
{
    CypressSession::validate();
    if (!isValidDate("dob", "yyyy-MM-dd"))
        throw ValidationError("dob");

    if (!isValidString("sex"))
        throw ValidationError("sex");
}

void TonometerSession::calculateInputs()
{
}

void TonometerSession::start()
{
    m_dialog = new TonometerDialog(nullptr, QSharedPointer<TonometerSession>(this));
    if (m_dialog == nullptr)
        throw QException();

    m_startDateTime = QDateTime::currentDateTimeUtc();
    m_status = SessionStatus::Started;

    m_dialog->run();
    m_dialog->show();

    qDebug() << "start session" << getSessionId() << m_startDateTime;
}

void TonometerSession::end()
{
    m_status = SessionStatus::Ended;
}
