#ifndef GENPROXYSESSION_H
#define GENPROXYSESSION_H

#include "cypress_session.h"

class GenProxySession : public CypressSession
{
public:
    explicit GenProxySession(QObject *parent, const QJsonObject& inputData);

    // CypressSession interface
public:
    void validate() const override;
    void calculateInputs() override;
    void start() override;
    void end() override;
};

#endif // GENPROXYSESSION_H
