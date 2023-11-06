#include "frax_session.h"
#include "dialogs/frax_dialog.h"

#include <stdexcept>

FraxSession::FraxSession(QObject* parent, const QJsonObject& inputData): CypressSession(parent, inputData)
{
}

void FraxSession::start()
{
    qDebug() << "start session" << getSessionId() << m_startDateTime;
    m_dialog = new FraxDialog(nullptr, QSharedPointer<FraxSession>(this));
    if (m_dialog == nullptr)
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

    qDebug() << m_inputData;

    if (!isValidInteger("age"))
        throw ValidationError("age");

    if (!isValidDate("dob", "yyyy-MM-dd"))
        throw ValidationError("dob");

    if (!isValidBool("alcohol"))
        throw ValidationError("alcohol");

    if (!isValidInteger("country_code"))
        throw ValidationError("country_code");

    if (!isValidBool("current_smoker"))
        throw ValidationError("current_smoker");

    if (!isValidBool("father_hip_fracture"))
        throw ValidationError("father_hip_fracture");

    if (!isValidBool("mother_hip_fracture"))
        throw ValidationError("mother_hip_fracture");

    if (!isValidDouble("femoral_neck_bmd"))
        throw ValidationError("femoral_neck_bmd");

    if (!isValidDouble("glucocorticoid_age") && !m_inputData.value("glucocorticoid_age").isUndefined() && !m_inputData.value("glucocorticoid_age").toString().isEmpty())
        throw ValidationError("glucocorticoid_age");

    if (!isValidDouble("glucocorticoid_number") && !m_inputData.value("glucocorticoid_number").isUndefined() && !m_inputData.value("glucocorticoid_number").toString().isEmpty())
        throw ValidationError("glucocorticoid_number");

    if (!isValidDouble("glucocorticoid_year") && !m_inputData.value("glucocorticoid_year").isUndefined() && !m_inputData.value("glucocorticoid_year").toString().isEmpty())
        throw ValidationError("glucocorticoid_year");

    if (!isValidBool("previous_fracture"))
        throw ValidationError("previous_fracture");

    if (!isValidString("ra_medications") && !m_inputData.value("ra_medications").isUndefined() && !m_inputData.value("ra_medications").toString().isEmpty())
        throw ValidationError("ra_medications"); // empty, None, or "dafadfkj,ad;fklad,adfkaldj"

    if (!isValidString("sex"))
        throw ValidationError("sex");

    if (!isValidString("type"))
        throw ValidationError("type");

    qDebug("validated");
}

void FraxSession::calculateInputs()
{
    validate();

    qDebug() << m_inputData;

    qDebug("calculate inputs");

    int age = m_inputData.value("age").toInt();
    qDebug() << "calculate inputs";
    if (age <= 0)
        throw ValidationError("age must be greater than 0");

    bool glucocorticoid = calculateGlucocorticoid(age);
    qDebug() << "glucco";

    bool father_hip_fracture = m_inputData.value("father_hip_fracture").toBool();
    qDebug() << "father hip";

    bool mother_hip_fracture = m_inputData.value("mother_hip_fracture").toBool();
    qDebug() << "mother hip";

    bool previous_fracture = father_hip_fracture || mother_hip_fracture;
    qDebug() << "previous fracture";

    double t_score = calculateTScore(m_inputData.value("femoral_neck_bmd").toDouble());
    qDebug() << "t score";

    m_inputData.insert("rheumatoid_arthritis", (m_inputData.value("ra_medications").toString() != "None") && (!m_inputData.value("ra_medications").toString().isEmpty()));
    m_inputData.insert("previous_fracture", previous_fracture);
    m_inputData.insert("glucocorticoid", glucocorticoid);
    m_inputData.insert("t_score", t_score);

    qDebug() << "glucocorticoid: " << glucocorticoid;
    qDebug() << "father_hip_fracture: " << father_hip_fracture;
    qDebug() << "mother_hip_fracture: " << mother_hip_fracture;
    qDebug() << "previous_fracture: " << previous_fracture;
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

bool FraxSession::calculateGlucocorticoid(int _age)
{
    int age = _age;

    int glucocorticoid_number = -1;
    int glucocorticoid_year = -1;
    int glucocorticoid_age = -1;


    if (!m_inputData.value("glucocorticoid_number").isNull())
    {
        glucocorticoid_number = m_inputData.value("glucocorticoid_number").toString().toInt();
    }


    if (!m_inputData.value("glucocorticoid_year").isNull())
    {
        glucocorticoid_year = m_inputData.value("glucocorticoid_year").toString().toInt();
    }

    if (!m_inputData.value("glucocorticoid_age").isNull())
    {
        glucocorticoid_age = m_inputData.value("glucocorticoid_age").toString().toInt();
    }

    int res = 0;

    if (age && glucocorticoid_number)
    {
        res = (age - glucocorticoid_number) <= 1 ? 1 : 0;
    }
    else if (glucocorticoid_year)
    {
        res = (QDate::currentDate().year() - glucocorticoid_year) <= 1 ? 1 : 0;
    }

    if (res == 1)
    {
        res = glucocorticoid_age >= 3 ? 1 : 0 ;
    }

    return res;
}