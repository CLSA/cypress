#ifndef CDTTSESSION_H
#define CDTTSESSION_H

#include "cypress_session.h"

class CDTTSession : public CypressSession
{
public:
    explicit CDTTSession(QObject *parent, const QJsonObject& inputData, const QString& origin);

    // CypressSession interface
public:
    void isInstalled() const override;
    void isAvailable() const override;
    void initializeDialog() override;
};

#endif // CDTTSESSION_H
