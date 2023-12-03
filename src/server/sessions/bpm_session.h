#ifndef BPMSESSION_H
#define BPMSESSION_H

#include "cypress_session.h"

class BPMSession : public CypressSession
{
public:
    explicit BPMSession(QObject *parent, const QJsonObject& inputData);

    // CypressSession interface
public:
    void start() override;
};

#endif // BPMSESSION_H
