#ifndef AUDIOMETERSESSION_H
#define AUDIOMETERSESSION_H

#include "cypress_session.h"

class AudiometerSession : public CypressSession
{
public:
    explicit AudiometerSession(QObject *parent, const QJsonObject& inputData);

    // CypressSession interface
public:
    void validate() const override;
    void calculateInputs() override;
    void start() override;
    void end() override;
};

#endif // AUDIOMETERSESSION_H
