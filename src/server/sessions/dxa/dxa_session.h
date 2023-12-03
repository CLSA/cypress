#ifndef DXASESSION_H
#define DXASESSION_H

#include "cypress_session.h"

class DXASession : public CypressSession
{
public:
    explicit DXASession(QObject *parent, const QJsonObject& inputData);

    // CypressSession interface
public:
    void start() override;
};

#endif // DXASESSION_H
