#include "easyone_connect_session.h"
#include "managers/spirometer/easyone_manager.h"
#include "dialogs/easyone_dialog.h"

EasyoneConnectSession::EasyoneConnectSession(QObject *parent, const QJsonObject& inputData, const QString& origin)
    : CypressSession{parent, inputData, origin}
{}

void EasyoneConnectSession::isInstalled() const
{
    if (!EasyoneConnectManager::isInstalled())
        throw NotInstalledError("Spirometer is not installed on this workstation");
}

void EasyoneConnectSession::isAvailable() const
{

}

void EasyoneConnectSession::validate() const
{
    CypressSession::validate();

    if (!isValidDate("dob", "yyyy-MM-dd"))
        throw ValidationError("dob");

    if (!isValidBool("smoker"))
        throw ValidationError("smoker");

    if (!isValidString("sex"))
        throw ValidationError("sex");

    if (!isValidDouble("height"))
        throw ValidationError("height");

    if (!isValidDouble("weight"))
        throw ValidationError("weight");
}

void EasyoneConnectSession::calculateInputs()
{
    CypressSession::calculateInputs();

    m_inputData["age"] = computeAge(m_inputData["dob"].toString());
    m_inputData["ethnicity"] = "caucasian";
}

void EasyoneConnectSession::initializeDialog()
{
    m_dialog = new EasyoneDialog(nullptr, QSharedPointer<EasyoneConnectSession>(this));
}

int EasyoneConnectSession::computeAge(const QString &birthdateStr)
{
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
