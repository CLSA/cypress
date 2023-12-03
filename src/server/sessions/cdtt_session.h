#ifndef CDTTSESSION_H
#define CDTTSESSION_H

#include "cypress_session.h"

class CDTTSession : public CypressSession
{
public:
    explicit CDTTSession(QObject *parent, const QJsonObject& inputData);

    // CypressSession interface
public:
    void start() override;
};

#endif // CDTTSESSION_H
