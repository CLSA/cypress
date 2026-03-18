#include "frax_helper.h"
#include "auxiliary/validators.h"
#include "auxiliary/utilities.h"

void FraxHelper::calculateAdditionalVariables(QJsonObject &input)
{
    const int age = input.value("age").toInt();
    qDebug() << "Age: " << age;
    if (age <= 0)
        throw Validators::ValidationError("age must be greater than 0");

    //// Get variables
    const double femoral_neck_bmd = input.value("femoral_neck_bmd").toDouble();
    const double weight = input.value("weight").toDouble();
    const double height = input.value("height").toDouble();

    qDebug()
        << "bmd"    << femoral_neck_bmd
        << "weight" << weight
        << "height" << height;

    const bool father_hip_fracture = input.value("father_hip_fracture").toBool();
    const bool mother_hip_fracture = input.value("mother_hip_fracture").toBool();

    qDebug()
        << "father_hip_fracture" << father_hip_fracture
        << "mother_hip_fracture" << mother_hip_fracture;

    const bool previous_fracture = input.value("previous_fracture").toBool();
    qDebug() << "previous_fracture:" << previous_fracture;

    //// Calculate new variables
    const bool parent_hip_fracture = father_hip_fracture || mother_hip_fracture;
    const int glucocorticoid_number = input.value("glucocorticoid_number").toInt();
    const int glucocorticoid_year = input.value("glucocorticoid_year").toInt();
    const int glucocorticoid_age = input.value("glucocorticoid_age").toInt();
    qDebug() << "glucocorticoid_number:" << glucocorticoid_number;
    qDebug() << "glucocorticoid_year:"   << glucocorticoid_year;
    qDebug() << "glucocorticoid_age:"    << glucocorticoid_age;

    const QString ra_medications = input.value("ra_medications").toString();
    qDebug() << "ra_medications:" << ra_medications;

    const double t_score = FraxHelper::calculateTScore(femoral_neck_bmd);
    qDebug() << "t_score:" << t_score;

    const double bmi = FraxHelper::calculateBmi(weight, height);
    qDebug() << "bmi:" << bmi;

    const bool rheumatoid_arthritis = ra_medications != "None" && !ra_medications.isEmpty();
    qDebug() << "rheumatoid_arthritis:" << rheumatoid_arthritis;

    const bool glucocorticoid = FraxHelper::calculateGlucocorticoid(
        age,
        glucocorticoid_number,
        glucocorticoid_year,
        glucocorticoid_age
    );
    qDebug() << "glucocorticoid: " << glucocorticoid;

    input.insert("rheumatoid_arthritis",     rheumatoid_arthritis);
    input.insert("previous_fracture",        previous_fracture);
    input.insert("secondary_osteoporosis",   false);
    input.insert("glucocorticoid",           glucocorticoid);
    input.insert("parent_hip_fracture",      parent_hip_fracture);
    input.insert("femoral_neck_tscore",      t_score);
    input.insert("body_mass_index",          bmi);
    input.insert("type",                     "t");
    input.insert("country_code",             "19");

    qDebug() << "Calculated additional variables";
    Utilities::prettyPrint(input);
}

double FraxHelper::calculateBmi(const double weightKg, const double heightCm)
{
    if (weightKg <= 0 || heightCm <= 0)
        throw Validators::ValidationError("Weight and height must be a positive number");

    double height_m = heightCm / 100.0;
    return weightKg / (height_m * height_m);
}

double FraxHelper::calculateTScore(const double femoralNeckBmd)
{
    const double M_value = 0.849;
    const double sigma = 0.111;
    const double bmd = femoralNeckBmd;
    const double t_score = ((bmd / M_value) - 1) * (M_value / sigma);

    return t_score;
}

double FraxHelper::calculateGlucocorticoid(
    const int age,
    const int glucocorticoid_number,
    const int glucocorticoid_year,
    const int glucocorticoid_age)
{
    int res = 0;

    if (age && glucocorticoid_number)
        res = (age - glucocorticoid_number) <= 1 ? 1 : 0;
    else if (glucocorticoid_year)
        res = (QDate::currentDate().year() - glucocorticoid_year) <= 1 ? 1 : 0;

    if (res == 1)
        res = glucocorticoid_age >= 3 ? 1 : 0;

    return res;
}
