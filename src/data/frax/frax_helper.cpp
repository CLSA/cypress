#include "frax_helper.h"
#include "auxiliary/validators.h"
#include "auxiliary/utilities.h"

void FraxHelper::calculateAdditionalVariables(QJsonObject &input)
{
    const int age = input.value("age").toInt();
    if (age <= 0)
        throw Validators::ValidationError("age must be greater than 0");

    //// Get variables
    const double femoral_neck_bmd = input.value("femoral_neck_bmd").toDouble();
    const double weight = input.value("weight").toDouble();
    const double height = input.value("height").toDouble();

    const bool father_hip_fracture = input.value("father_hip_fracture").toBool();
    const bool mother_hip_fracture = input.value("mother_hip_fracture").toBool();

    // Derived
    const bool parent_hip_fracture = father_hip_fracture || mother_hip_fracture;

    const int glucocorticoid_number = input.value("glucocorticoid_number").toInt();
    const int glucocorticoid_year = input.value("glucocorticoid_year").toInt();
    const int glucocorticoid_age = input.value("glucocorticoid_age").toInt();

    // Derived
    const bool glucocorticoid = FraxHelper::calculateGlucocorticoid(
        age,
        glucocorticoid_number,
        glucocorticoid_year,
        glucocorticoid_age
    );

    const QString ra_medications = input.value("ra_medications").toString();

    // Derived
    const bool rheumatoid_arthritis = ra_medications.toLower() != "none" && !ra_medications.isEmpty();

    const double t_score = FraxHelper::calculateTScore(femoral_neck_bmd);
    const double bmi = FraxHelper::calculateBmi(weight, height);

    input.insert("type",                     "t");
    input.insert("country_code",             "19");
    input.insert("rheumatoid_arthritis",     rheumatoid_arthritis);
    input.insert("secondary_osteoporosis",   false);
    input.insert("glucocorticoid",           glucocorticoid);
    input.insert("parent_hip_fracture",      parent_hip_fracture);
    input.insert("femoral_neck_tscore",      t_score);
    input.insert("body_mass_index",          bmi);


    Utilities::prettyPrint(input);
}

double FraxHelper::calculateBmi(const double weightKg, const double heightCm)
{
    if (weightKg <= 0 || heightCm <= 0)
        throw Validators::ValidationError("Weight and height must be a positive number");

    double height_m = heightCm / 100.0;
    return weightKg / (height_m * height_m);
}

double FraxHelper::calculateTScore(const double bmd)
{
    double M = 0.849f;     // median age matched reference value
    double L = 1.0f;       // skewness of the LMS reference curve corresponding to age peak BMD
    double sigma = 0.111f; // age matched population standard deviation
    return ((M * ((bmd / M ) - 1.0f))/(L * sigma));
}

double FraxHelper::calculateGlucocorticoid(
    const int age,                    // age in years
    const int glucocorticoid_number,  // how many months over your entire life did you take them
    const int glucocorticoid_year,    // at what year did you last take them
    const int glucocorticoid_age)     // at what age (yrs) did you last use them
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
