#include "spirometer_session.h"

#include "dialogs/spirometer_dialog.h"

SpirometerSession::SpirometerSession(QObject *parent, const QJsonObject& inputData)
    : CypressSession{parent, inputData}
{

}

void SpirometerSession::validate() const
{
    CypressSession::validate();

    qDebug() << m_inputData;
    //if (!isValidString("age"))
    //    throw ValidationError("age");
    //if (!isValidString("ethnicity"))
    //    throw ValidationError("ethnicity");
    if (!isValidDate("dob", "yyyy-MM-dd"))
        throw ValidationError("dob");

    if (!isValidBool("asthma"))
        throw ValidationError("asthma");

    if (!isValidBool("smoker"))
        throw ValidationError("smoker");

    if (!isValidBool("copd"))
        throw ValidationError("copd");

    if (!isValidString("sex"))
        throw ValidationError("sex");

    if (!isValidDouble("height"))
        throw ValidationError("height");

    if (!isValidDouble("weight"))
        throw ValidationError("weight");
}

int computeAge(const QString &birthdateStr) {
    QDate birthdate = QDate::fromString(birthdateStr, "yyyy-MM-dd");
    QDate currentDate = QDate::currentDate();

    int years = currentDate.year() - birthdate.year();

    // Subtract a year if the birthday hasn't occurred yet this year
    if ((birthdate.month() > currentDate.month()) ||
        ((birthdate.month() == currentDate.month()) &&
         (birthdate.day() > currentDate.day()))) {
        years--;
    }

    return years;
}

void SpirometerSession::calculateInputs()
{
    // TODO calculate age from DOB
    m_inputData["age"] = computeAge(m_inputData["dob"].toString());
    m_inputData["ethnicity"] = "caucasian";
}

void SpirometerSession::start()
{
    m_dialog = new SpirometerDialog(nullptr, QSharedPointer<SpirometerSession>(this));
    if (m_dialog == nullptr)
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