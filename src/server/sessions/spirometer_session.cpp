#include "spirometer_session.h"

#include "dialogs/spirometer_dialog.h"

SpirometerSession::SpirometerSession(QObject *parent, const QJsonObject& inputData)
    : CypressSession{parent, inputData}
{

}

void SpirometerSession::validate() const
{
    CypressSession::validate();

    if (!isValidString("age"))
        throw ValidationError("age");
    if (!isValidString("ethnicity"))
        throw ValidationError("ethnicity");
    if (!isValidString("date_of_birth"))
        throw ValidationError("date_of_birth");
    if (!isValidString("smoker"))
        throw ValidationError("date_of_birth");
    if (!isValidString("copd"))
        throw ValidationError("date_of_birth");
    if (!isValidString("gender"))
        throw ValidationError("date_of_birth");
}

void SpirometerSession::calculateInputs()
{

}

void SpirometerSession::start()
{
    m_dialog.reset(new SpirometerDialog(nullptr, *this));
    if (m_dialog.isNull())
        throw QException();

    m_startDateTime = QDateTime::currentDateTimeUtc();
    m_status = SessionStatus::Started;

    m_dialog->run();
    m_dialog->show();

    qDebug() << "start session" << getSessionId() << m_startDateTime;
}

void SpirometerSession::end()
{

}
