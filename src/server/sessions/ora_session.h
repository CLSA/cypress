#ifndef ORA_SESSION_H
#define ORA_SESSION_H

#include <QObject>

#include "cypress_session.h"

class ORASession : public CypressSession
{
public:
    ORASession(QObject* parent, const QJsonObject& inputData, const QString& origin);

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

#endif // ORA_SESSION_H
