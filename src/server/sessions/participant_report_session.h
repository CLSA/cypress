#ifndef PARTICIPANTREPORTSESSION_H
#define PARTICIPANTREPORTSESSION_H

#include "cypress_session.h"

class ParticipantReportSession : public CypressSession
{
public:
    explicit ParticipantReportSession(QObject *parent, const QJsonObject& inputData);

    // CypressSession interface
public:
    void validate() const override;
    void calculateInputs() override;
    void start() override;
};

#endif // PARTICIPANTREPORTSESSION_H
