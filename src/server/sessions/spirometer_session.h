#ifndef SPIROMETERSESSION_H
#define SPIROMETERSESSION_H

#include "cypress_session.h"

class SpirometerSession : public CypressSession
{
public:
    explicit SpirometerSession(QObject *parent, const QJsonObject& inputData);

    // CypressSession interface
public:
    void validate() const override;
    void calculateInputs() override;
    void start() override;
    void end() override;
};

#endif // SPIROMETERSESSION_H
