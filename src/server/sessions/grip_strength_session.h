#ifndef GRIPSTRENGTHSESSION_H
#define GRIPSTRENGTHSESSION_H

#include "cypress_session.h"

class GripStrengthSession : public CypressSession
{
public:
    explicit GripStrengthSession(QObject *parent, const QJsonObject& inputData);

    // CypressSession interface
public:
    void validate() const override;
    void calculateInputs() override;
    void start() override;
    void end() override;
};

#endif // GRIPSTRENGTHSESSION_H
