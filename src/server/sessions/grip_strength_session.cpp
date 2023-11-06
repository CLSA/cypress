#include "grip_strength_session.h"

#include "dialogs/grip_strength_dialog.h"

GripStrengthSession::GripStrengthSession(QObject *parent, const QJsonObject& inputData)
    : CypressSession{parent, inputData}
{

}

void GripStrengthSession::validate() const
{
    CypressSession::validate();
}

void GripStrengthSession::calculateInputs()
{

}

void GripStrengthSession::start()
{
    m_dialog = new GripStrengthDialog(nullptr, QSharedPointer<GripStrengthSession>(this));
    if (m_dialog == nullptr)
        throw QException();

    m_startDateTime = QDateTime::currentDateTimeUtc();
    m_status = SessionStatus::Started;

    m_dialog->run();
    m_dialog->show();

    qDebug() << "start session" << getSessionId() << m_startDateTime;
}

void GripStrengthSession::end()
{

}
