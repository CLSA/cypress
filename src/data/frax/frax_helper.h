#ifndef FRAX_HELPER_H
#define FRAX_HELPER_H

#include <QJsonObject>

namespace FraxHelper
{
    /**
     * @brief writeInputFile
     * @param input
     */
    void writeInputFile(const QJsonObject& input);

    /**
     * @brief readOutputFile
     * @return
     */
    QJsonObject readOutputFile();

    /**
     * @brief calculateAdditionalVariables
     * @param input
     */
    void calculateAdditionalVariables(QJsonObject& input);

    /**
     * @brief calculateBmi
     * @param weightKg
     * @param heightCm
     * @return
     */
    double calculateBmi(const double weightKg, const double heightCm);

    /**
     * @brief calculateTScore
     * @param femoralNeckBmd
     * @return
     */
    double calculateTScore(const double femoralNeckBmd);

    /**
     * @brief calculateGlucocorticoid
     * @param age
     * @param glucocorticoid_number
     * @param glucocorticoid_year
     * @param glucocorticoid_age
     * @return
     */
    double calculateGlucocorticoid(
        const int age,
        const int glucocorticoid_number,
        const int glucocorticoid_year,
        const int glucocorticoid_age);
}

#endif // FRAX_HELPER_H
