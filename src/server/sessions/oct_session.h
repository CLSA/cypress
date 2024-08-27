#ifndef OCT_SESSION_H
#define OCT_SESSION_H

#include "cypress_session.h"


class OCTSession : public CypressSession
{
public:
    enum Side { Left, Right };

    explicit OCTSession(QObject* parent, const QJsonObject& inputData, const QString& origin, OCTSession::Side side);

    OCTSession::Side getSide();

    // CypressSession interface
    void initializeDialog() override;
    void isInstalled() const override;
    void isAvailable() const override;

private:
    OCTSession::Side m_side { Left };
};

#endif // OCT_SESSION_H
