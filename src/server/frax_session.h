#ifndef FRAXSESSION_H
#define FRAXSESSION_H

#include <cypress_session.h>

class FraxSession : public CypressSession
{
public:
    FraxSession(const QJsonObject& inputData);

    void validate() const override;
    void calculateInputs() override;

// CypressSession interface
protected:


    double calculateBMI(double weight_kg, double height_cm);
    double calculateTScore(double femoral_neck_bmd);
    bool calculateGlucocorticoid(int age);
};

#endif // FRAXSESSION_H
