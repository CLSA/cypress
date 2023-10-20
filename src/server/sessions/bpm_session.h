#ifndef BPMSESSION_H
#define BPMSESSION_H

#include "cypress_session.h"

class BPMSession : public CypressSession
{
public:
    explicit BPMSession(QObject *parent, const QJsonObject& inputData);

    // CypressSession interface
public:
    void validate() const override;
    void calculateInputs() override;
    void start() override;
    void end() override;
};

#endif // BPMSESSION_H
