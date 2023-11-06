#ifndef ULTRASOUNDSESSION_H
#define ULTRASOUNDSESSION_H

#include "cypress_session.h"

#include <QJsonObject>

class UltrasoundSession : public CypressSession
{
public:
    explicit UltrasoundSession(QObject *parent, const QJsonObject& inputData);
    ~UltrasoundSession();

    // CypressSession interface
public:
    void validate() const override;
    void calculateInputs() override;
    void start() override;
    void end() override;
};

#endif // ULTRASOUNDSESSION_H
