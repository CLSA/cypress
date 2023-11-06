#include "choice_reaction_session.h"
#include "dialogs/choice_reaction_dialog.h"

ChoiceReactionSession::ChoiceReactionSession(QObject *parent, const QJsonObject& inputData)
    : CypressSession{parent, inputData}
{

}

void ChoiceReactionSession::validate() const
{
    CypressSession::validate();
}

void ChoiceReactionSession::calculateInputs()
{

}

void ChoiceReactionSession::start()
{
    m_dialog = new ChoiceReactionDialog(nullptr, QSharedPointer<ChoiceReactionSession>(this));
    if (m_dialog == nullptr)
        throw QException();

    m_startDateTime = QDateTime::currentDateTimeUtc();
    m_status = SessionStatus::Started;

    m_dialog->run();
    m_dialog->show();

    qDebug() << "start session" << getSessionId() << m_startDateTime;
}

void ChoiceReactionSession::end()
{

}
