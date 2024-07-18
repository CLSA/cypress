#ifndef HEARCON_SESSION_H
#define HEARCON_SESSION_H

#include "cypress_session.h"

class HearconSession : public CypressSession
{
public:
    explicit HearconSession(QObject *parent, const QJsonObject& inputData, const QString& origin);

public:
    void initializeDialog() override;
    void isInstalled() const override;
    void isAvailable() const override;
};

#endif // HEARCON_SESSION_H
