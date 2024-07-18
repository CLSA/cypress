#ifndef WATCH_BP_SESSION_H
#define WATCH_BP_SESSION_H

#include <cypress_session.h>

class WatchBPSession : public CypressSession
{
public:
    explicit WatchBPSession(QObject *parent, const QJsonObject& inputData, const QString& origin);

    // CypressSession interface
public:
    void isInstalled() const override;
    void isAvailable() const override;
    void initializeDialog() override;
    void validate() const override;
    void calculateInputs() override;
};

#endif // WATCH_BP_SESSION_H
