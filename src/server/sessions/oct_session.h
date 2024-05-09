#ifndef OCT_SESSION_H
#define OCT_SESSION_H

#include "cypress_session.h"

class OCTSession : public CypressSession
{
public:
    explicit OCTSession(QObject* parent, const QJsonObject& inputData, const QString& origin);

    // CypressSession interface
public:
    void initializeDialog() override;
    void isInstalled() const override;
    void isAvailable() const override;
};

#endif // OCT_SESSION_H
