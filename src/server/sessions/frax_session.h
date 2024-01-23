#ifndef FRAXSESSION_H
#define FRAXSESSION_H

#include "cypress_session.h"

class FraxSession : public CypressSession
{
public:
    FraxSession(QObject* parent, const QJsonObject& inputData);



// CypressSession interface
protected:
    void validate() const override;
    void calculateInputs() override;
    void isInstalled() const override;
    void isAvailable() const override;

    double calculateBMI(double weight_kg, double height_cm);
    double calculateTScore(double femoral_neck_bmd);
    bool calculateGlucocorticoid(int age);

    void initializeDialog() override;
};

#endif // FRAXSESSION_H
