#ifndef EASYONE_CONNECT_SESSION_H
#define EASYONE_CONNECT_SESSION_H

#include "cypress_session.h"

class EasyoneConnectSession : public CypressSession
{
public:
    explicit EasyoneConnectSession(QObject *parent, const QJsonObject& inputData, const QString& origin);

public:
    void isInstalled() const override;
    void isAvailable() const override;
    void validate() const override;

    void calculateInputs() override;

    void initializeDialog() override;
private:
    int computeAge(const QString &birthdateStr);
};

#endif // EASYONE_CONNECT_SESSION_H
