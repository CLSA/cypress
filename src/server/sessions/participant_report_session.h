#ifndef PARTICIPANTREPORTSESSION_H
#define PARTICIPANTREPORTSESSION_H

#include "cypress_session.h"

class ParticipantReportSession : public CypressSession
{
public:
    explicit ParticipantReportSession(QObject *parent, const QJsonObject& inputData, const QString& origin);

    // CypressSession interface
public:
    void validate() const override;
    void calculateInputs() override;
    void start() override;
    void initializeDialog() override;
};

#endif // PARTICIPANTREPORTSESSION_H
