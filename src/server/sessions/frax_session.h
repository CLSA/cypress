#ifndef FRAXSESSION_H
#define FRAXSESSION_H

#include "cypress_session.h"

class FraxSession : public CypressSession
{
public:
    FraxSession(QObject* parent, const QJsonObject& inputData, const QString& origin);

// CypressSession interface
protected:
    void validate() const override;
    void calculateInputs() override;
    void isInstalled() const override;
    void isAvailable() const override;
    void initializeDialog() override;

    //double calculateBMI(double weight_kg, double height_cm) const;
    //double calculateTScore(double femoral_neck_bmd) const;
    //bool calculateGlucocorticoid(int age);
};

#endif // FRAXSESSION_H
