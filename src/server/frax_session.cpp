#include "frax_session.h"
#include "dialogs/frax_dialog.h"

#include <stdexcept>

FraxSession::FraxSession(const QJsonObject& inputData): CypressSession(inputData)
{
}

void FraxSession::start()
{
    m_dialog.reset(new FraxDialog(nullptr, *this));
    if (m_dialog.isNull())
        throw QException();

    m_startDateTime = QDateTime::currentDateTimeUtc();
    m_status = SessionStatus::Started;

    m_dialog->run();
    m_dialog->show();

    qDebug() << "start session" << getSessionId() << m_startDateTime;
}

void FraxSession::validate() const
{
    CypressSession::validate();

    //if (!isValidString("age"))
    //    throw ValidationError("age");

    //if (!isValidDate("dob", "yyyy-mm-dd"))
    //    throw ValidationError("dob");

    //if (!isValidString("alcohol"))
    //    throw ValidationError("alcohol");

    //if (!isValidString("country_code"))
    //    throw ValidationError("country_code");

    //if (!isValidBool("current_smoker"))
    //    throw ValidationError("current_smoker");

    //if (!isValidString("father_hip_fracture"))
    //    throw ValidationError("father_hip_fracture");

    //if (!isValidString("mother_hip_fracture"))
    //    throw ValidationError("mother_hip_fracture");

    //if (!isValidString("femoral_neck_bmd"))
    //    throw ValidationError("femoral_neck_bmd");

    //if (!isValidString("glucocorticoid_age"))
    //    throw ValidationError("glucocorticoid_age");

    //if (!isValidString("glucocorticoid_number"))
    //    throw ValidationError("glucocorticoid_number");

    //if (!isValidDate("glucocorticoid_year", "yyyy-mm-dd"))
    //    throw ValidationError("glucocorticoid_year");

    //if (!isValidString("height"))
    //    throw ValidationError("height");

    //if (!isValidString("previous_fracture"))
    //    throw ValidationError("previous_fracture");

    //if (!isValidString("rheumatoid_arthritis"))
    //    throw ValidationError("rheumatoid_arthritis");

    //if (!isValidString("sex"))
    //    throw ValidationError("sex");

    //if (!isValidString("type"))
    //    throw ValidationError("type");

    //if (!isValidString("weight"))
    //    throw ValidationError("weight");
}

void FraxSession::calculateInputs()
{
    //validate();

    //int age = m_inputData.value("age").toInt();
    //if (age <= 0)
    //    throw std::invalid_argument("age must be greater than 0");

    //double weight = m_inputData.value("weight").toDouble();
    //if (weight <= 0)
    //    throw std::invalid_argument("weight must be greater than 0");

    //double height = m_inputData.value("height").toDouble();
    //if (height <= 0)
    //    throw std::invalid_argument("height must be greater than 0");

    //double bmi = calculateBMI(weight, height);
    //bool glucocorticoid = calculateGlucocorticoid(age);
    //bool father_hip_fracture = m_inputData.value("father_hip_fracture").toString() == "Yes" ? true : false;
    //bool mother_hip_fracture = m_inputData.value("mother_hip_fracture").toString() == "Yes" ? true : false;
    //bool previous_fracture = father_hip_fracture || mother_hip_fracture;
    //double t_score = calculateTScore(m_inputData.value("femoral_neck_bmd").toDouble());

    //m_inputData.insert("bmi", bmi);
    //m_inputData.insert("previous_fracture", previous_fracture);
    //m_inputData.insert("glucocorticoid", glucocorticoid);
    //m_inputData.insert("t_score", t_score);

    //qDebug() << "bmi: " << bmi;
    //qDebug() << "glucocorticoid: " << glucocorticoid;
    //qDebug() << "father_hip_fracture: " << father_hip_fracture;
    //qDebug() << "mother_hip_fracture: " << mother_hip_fracture;
    //qDebug() << "previous_fracture: " << previous_fracture;
}

double FraxSession::calculateBMI(double weight_kg, double height_cm)
{
    if (weight_kg <= 0 || height_cm <= 0)
    {
        throw std::invalid_argument("Weight and height must be a positive number");
    }

    double height_m = height_cm / 100.0;

    return weight_kg / (height_m * height_m);
}

double FraxSession::calculateTScore(double femoral_neck_bmd)
{
    double M_value = 0.849;
    // double L_value = 1.0;
    double sigma = 0.111;
    double bmd = femoral_neck_bmd;

    double t_score = ((bmd / M_value) - 1) * (M_value / sigma);

    return t_score;
}

bool FraxSession::calculateGlucocorticoid(int age)
{
    int v2 = age;
    int v3 = m_inputData.value("glucocorticoid_number").toInt();
    int v4 = m_inputData.value("glucocorticoid_year").toInt();
    int v5 = m_inputData.value("glucocorticoid_age").toInt();

    int res = 0;

    if (v2 && v3)
    {
        res = (v2 - v3) <= 1 ? 1 : 0;
    }
    else if (v4)
    {
        res = (QDate::currentDate().year() - v4) <= 1 ? 1 : 0;
    }

    if (res == 1)
    {
        res = v5 >= 3 ? 1 : 0 ;
    }

    return res;
}
