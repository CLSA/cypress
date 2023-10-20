#ifndef RETINALCAMERASESSION_H
#define RETINALCAMERASESSION_H

#include "cypress_session.h"

class RetinalCameraSession : public CypressSession
{
public:
    explicit RetinalCameraSession(QObject *parent, const QJsonObject& inputData);

    // CypressSession interface
public:
    void validate() const override;
    void calculateInputs() override;
    void start() override;
    void end() override;
};

#endif // RETINALCAMERASESSION_H
