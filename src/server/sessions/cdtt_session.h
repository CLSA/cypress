#ifndef CDTTSESSION_H
#define CDTTSESSION_H

#include "cypress_session.h"

class CDTTSession : public CypressSession
{
public:
    explicit CDTTSession(QObject *parent, const QJsonObject& inputData);

    // CypressSession interface
public:
    void validate() const override;
    void calculateInputs() override;
    void start() override;
    void end() override;
};

#endif // CDTTSESSION_H
