#ifndef CHOICE_REACTION_MEASUREMENT_H
#define CHOICE_REACTION_MEASUREMENT_H

#include "data/measurement.h"

/*!
* \class ChoiceReationMeasurement
* \brief A ChoiceReationMeasurement class
*
* Measurements of choice reaction are derived from a
* touch screen monitor running the CBB test executable.
* This class facilitates parsing comma delimited data from
* the csv output of a test run.
*
* \sa Measurement
*/

class ChoiceReactionMeasurement : public Measurement
{
public:
    ChoiceReactionMeasurement() = default;
    ~ChoiceReactionMeasurement() = default;

    void fromString(const QString &);

    bool isValid() const override;

    QString toString() const override;

    static const int TEST_CODE;
};

Q_DECLARE_METATYPE(ChoiceReactionMeasurement);

QDebug operator<<(QDebug dbg, const ChoiceReactionMeasurement &);

#endif // CHOICE_REACTION_MEASUREMENT_H
