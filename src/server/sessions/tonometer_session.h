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
    void start() override;
};

#endif // TONOMETERSESSION_H
