#ifndef CHOICEREACTIONSESSION_H
#define CHOICEREACTIONSESSION_H

#include "cypress_session.h"

class ChoiceReactionSession : public CypressSession
{
public:
    explicit ChoiceReactionSession(QObject *parent, const QJsonObject& inputData);

    // CypressSession interface
public:
    void validate() const override;
    void calculateInputs() override;
    void start() override;
    void end() override;
};

#endif // CHOICEREACTIONSESSION_H
