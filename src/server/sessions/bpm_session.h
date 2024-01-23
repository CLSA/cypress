#ifndef BPMSESSION_H
#define BPMSESSION_H

#include "cypress_session.h"

class BPMSession : public CypressSession
{
public:
    explicit BPMSession(QObject *parent, const QJsonObject& inputData);

public:
    void isInstalled() const override;
    void isAvailable() const override;

    void initializeDialog() override;
};

#endif // BPMSESSION_H
