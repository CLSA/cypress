#ifndef TONOMETERSESSION_H
#define TONOMETERSESSION_H

#include "cypress_session.h"

class TonometerSession : public CypressSession
{
public:
    explicit TonometerSession(QObject *parent, const QJsonObject& inputData);

    // CypressSession interface
public:
    void validate() const override;
    void calculateInputs() override;
    void start() override;
    void end() override;
};

#endif // TONOMETERSESSION_H
