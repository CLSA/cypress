#include "frax_session.h"

#include "data/frax/frax_helper.h"
#include "dialogs/frax_dialog.h"
#include "managers/frax/frax_manager.h"
#include "auxiliary/validators.h"

FraxSession::FraxSession(QObject* parent, const QJsonObject& inputData, const QString& origin)
    : CypressSession(parent, inputData, origin)
{
}

void FraxSession::initializeDialog()
{
    m_dialog = new FraxDialog(nullptr, QSharedPointer<FraxSession>(this));
}

void FraxSession::validate() const
{
    CypressSession::validate();

    //qDebug() << m_inputData;

    if (!Validators::isValidInteger(m_inputData, "age"))
        throw ValidationError("age is not an integer");

    if (!Validators::isValidBool(m_inputData, "alcohol"))
        throw ValidationError("alcohol is not a bool");

    if (!Validators::isValidBool(m_inputData, "current_smoker"))
        throw ValidationError("current_smoker is not a bool");

    if (!Validators::isValidBool(m_inputData, "father_hip_fracture"))
        throw ValidationError("father_hip_fracture is not a bool");

    if (!Validators::isValidBool(m_inputData, "mother_hip_fracture"))
        throw ValidationError("mother_hip_fracture is not a bool");

    if (!Validators::isValidDouble(m_inputData, "weight"))
        throw ValidationError("weight is not a double");

    if (m_inputData.value("weight").toDouble() <= 0)
        throw ValidationError("weight must be greater than 0");

    if (!Validators::isValidDouble(m_inputData, "height"))
        throw ValidationError("height is not a double");

    if (m_inputData.value("height").toDouble() <= 0)
        throw ValidationError("height must be greater than 0");

    if (!Validators::isValidDouble(m_inputData, "femoral_neck_bmd"))
        throw ValidationError("femoral_neck_bmd is not a double");

    if (!Validators::isValidInteger(m_inputData, "glucocorticoid_age") && !m_inputData.value("glucocorticoid_age").isUndefined() && !m_inputData.value("glucocorticoid_age").toString().isEmpty())
        throw ValidationError("glucocorticoid_age");

    if (!Validators::isValidInteger(m_inputData, "glucocorticoid_number") && !m_inputData.value("glucocorticoid_number").isUndefined() && !m_inputData.value("glucocorticoid_number").toString().isEmpty())
        throw ValidationError("glucocorticoid_number");

    if (!Validators::isValidInteger(m_inputData, "glucocorticoid_year") && !m_inputData.value("glucocorticoid_year").isUndefined() && !m_inputData.value("glucocorticoid_year").toString().isEmpty())
        throw ValidationError("glucocorticoid_year");

    if (!Validators::isValidBool(m_inputData, "previous_fracture"))
        throw ValidationError("previous_fracture is not a bool");

    if (!Validators::isValidString(m_inputData, "ra_medications") && !m_inputData.value("ra_medications").isUndefined() && !m_inputData.value("ra_medications").toString().isEmpty())
        throw ValidationError("ra_medications is not a valid string"); // empty, None, or "dafadfkj,ad;fklad,adfkaldj"

    if (!Validators::isValidString(m_inputData, "sex"))
        throw ValidationError("sex is not a valid string");
}

void FraxSession::calculateInputs()
{
    validate();

    FraxHelper::calculateAdditionalVariables(m_inputData);
}

void FraxSession::isInstalled() const
{
    if (FraxManager::config->hasErrors())
        throw NotInstalledError("FRAX is not installed on this workstation");
}

void FraxSession::isAvailable() const
{

}

void FraxSession::setInputData(const QString& key, const QJsonValue& val) {
    m_inputData[key] = val;
}
