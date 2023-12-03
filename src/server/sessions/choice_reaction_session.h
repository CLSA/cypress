#ifndef CHOICEREACTIONSESSION_H
#define CHOICEREACTIONSESSION_H

#include "cypress_session.h"

class ChoiceReactionSession : public CypressSession
{
public:
    explicit ChoiceReactionSession(QObject *parent, const QJsonObject& inputData);

    // CypressSession interface
public:
    void isInstalled() const override;
    void isAvailable() const override;

    void start() override;
};

#endif // CHOICEREACTIONSESSION_H
