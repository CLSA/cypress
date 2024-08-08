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

    qDebug() << m_inputData;

    if (!Validators::isValidInteger(m_inputData, "age"))
        throw ValidationError("age");

    if (!Validators::isValidBool(m_inputData, "alcohol"))
        throw ValidationError("alcohol");

    if (!Validators::isValidBool(m_inputData, "current_smoker"))
        throw ValidationError("current_smoker");

    if (!Validators::isValidBool(m_inputData, "father_hip_fracture"))
        throw ValidationError("father_hip_fracture");

    if (!Validators::isValidBool(m_inputData, "mother_hip_fracture"))
        throw ValidationError("mother_hip_fracture");

    if (!Validators::isValidDouble(m_inputData, "weight"))
        throw ValidationError("weight");

    if (!Validators::isValidDouble(m_inputData, "height"))
        throw ValidationError("height");

    if (!Validators::isValidDouble(m_inputData, "femoral_neck_bmd"))
        throw ValidationError("femoral_neck_bmd");

    if (!Validators::isValidInteger(m_inputData, "glucocorticoid_age") && !m_inputData.value("glucocorticoid_age").isUndefined() && !m_inputData.value("glucocorticoid_age").toString().isEmpty())
        throw ValidationError("glucocorticoid_age");

    if (!Validators::isValidInteger(m_inputData, "glucocorticoid_number") && !m_inputData.value("glucocorticoid_number").isUndefined() && !m_inputData.value("glucocorticoid_number").toString().isEmpty())
        throw ValidationError("glucocorticoid_number");

    if (!Validators::isValidInteger(m_inputData, "glucocorticoid_year") && !m_inputData.value("glucocorticoid_year").isUndefined() && !m_inputData.value("glucocorticoid_year").toString().isEmpty())
        throw ValidationError("glucocorticoid_year");

    if (!Validators::isValidBool(m_inputData, "previous_fracture"))
        throw ValidationError("previous_fracture");

    if (!Validators::isValidString(m_inputData, "ra_medications") && !m_inputData.value("ra_medications").isUndefined() && !m_inputData.value("ra_medications").toString().isEmpty())
        throw ValidationError("ra_medications"); // empty, None, or "dafadfkj,ad;fklad,adfkaldj"

    if (!Validators::isValidString(m_inputData, "sex"))
        throw ValidationError("sex");
}

void FraxSession::calculateInputs()
{
    validate();
    FraxHelper::calculateAdditionalVariables(m_inputData);

    //int age = m_inputData.value("age").toInt();
    //if (age <= 0)
    //    throw ValidationError("age must be greater than 0");

    //bool glucocorticoid = calculateGlucocorticoid(age);
    //bool father_hip_fracture = m_inputData.value("father_hip_fracture").toBool();
    //bool mother_hip_fracture = m_inputData.value("mother_hip_fracture").toBool();
    //bool previous_fracture = father_hip_fracture || mother_hip_fracture;
    //double t_score = calculateTScore(m_inputData.value("femoral_neck_bmd").toDouble());
    //double bmi = calculateBMI(m_inputData.value("weight").toDouble(), m_inputData.value("height").toDouble());

    //m_inputData.insert("rheumatoid_arthritis",   (m_inputData.value("ra_medications").toString() != "None") && (!m_inputData.value("ra_medications").toString().isEmpty()));
    //m_inputData.insert("previous_fracture",      previous_fracture);
    //m_inputData.insert("secondary_osteoporosis", false);
    //m_inputData.insert("glucocorticoid",         glucocorticoid);
    //m_inputData.insert("parent_hip_fracture",    father_hip_fracture || mother_hip_fracture);
    //m_inputData.insert("femoral_neck_bmd",       t_score);
    //m_inputData.insert("body_mass_index",        bmi);
    //m_inputData.insert("type",                  "t");
    //m_inputData.insert("country_code",           "19");

    //qDebug() << "glucocorticoid: " << glucocorticoid;
    //qDebug() << "father_hip_fracture: " << father_hip_fracture;
    //qDebug() << "mother_hip_fracture: " << mother_hip_fracture;
    //qDebug() << "previous_fracture: " << previous_fracture;
}

void FraxSession::isInstalled() const
{
    if (FraxManager::config.hasErrors())
        throw NotInstalledError("FRAX is not installed on this workstation");
}

void FraxSession::isAvailable() const
{

}

//double FraxSession::calculateBMI(double weight_kg, double height_cm) const
//{
//    if (weight_kg <= 0 || height_cm <= 0)
//        throw std::invalid_argument("Weight and height must be a positive number");
//
//    double height_m = height_cm / 100.0;
//    return weight_kg / (height_m * height_m);
//}
//
//double FraxSession::calculateTScore(double femoral_neck_bmd) const
//{
//    const double M_value = 0.849;
//    const double sigma = 0.111;
//    const double bmd = femoral_neck_bmd;
//    const double t_score = ((bmd / M_value) - 1) * (M_value / sigma);
//
//    return t_score;
//}
//
//bool FraxSession::calculateGlucocorticoid(int _age)
//{
//    int age = _age;
//    int glucocorticoid_number = -1;
//    int glucocorticoid_year = -1;
//    int glucocorticoid_age = -1;
//
//    if (!m_inputData.value("glucocorticoid_number").isNull())
//        glucocorticoid_number = m_inputData.value("glucocorticoid_number").toString().toInt();
//
//    if (!m_inputData.value("glucocorticoid_year").isNull())
//        glucocorticoid_year = m_inputData.value("glucocorticoid_year").toString().toInt();
//
//    if (!m_inputData.value("glucocorticoid_age").isNull())
//        glucocorticoid_age = m_inputData.value("glucocorticoid_age").toString().toInt();
//
//    int res = 0;
//
//    if (age && glucocorticoid_number)
//        res = (age - glucocorticoid_number) <= 1 ? 1 : 0;
//    else if (glucocorticoid_year)
//        res = (QDate::currentDate().year() - glucocorticoid_year) <= 1 ? 1 : 0;
//    if (res == 1)
//        res = glucocorticoid_age >= 3 ? 1 : 0;
//
//    return res;
//}
