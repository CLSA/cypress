#include "spirometer_session.h"
#include "managers/spirometer/spirometer_manager.h"

#include "dialogs/spirometer_dialog.h"

SpirometerSession::SpirometerSession(QObject *parent, const QJsonObject& inputData)
    : CypressSession{parent, inputData}
{
}

void SpirometerSession::initializeDialog()
{
    m_dialog = new SpirometerDialog(nullptr, QSharedPointer<SpirometerSession>(this));
}

void SpirometerSession::isInstalled() const {
    if (!SpirometerManager::isInstalled())
        throw NotInstalledError("Spirometer is not installed on this workstation");
}

void SpirometerSession::isAvailable() const {

}

void SpirometerSession::validate() const
{
    CypressSession::validate();

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

void SpirometerSession::calculateInputs()
{
    CypressSession::calculateInputs();

    m_inputData["age"] = computeAge(m_inputData["dob"].toString());
    m_inputData["ethnicity"] = "caucasian";
}

int SpirometerSession::computeAge(const QString &birthdateStr) {
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


