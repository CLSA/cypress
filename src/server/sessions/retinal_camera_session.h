#ifndef RETINALCAMERASESSION_H
#define RETINALCAMERASESSION_H

#include "cypress_session.h"

enum Side { Left, Right };

class RetinalCameraSession : public CypressSession
{
public:
    explicit RetinalCameraSession(QObject *parent,
                                  const QJsonObject &inputData,
                                  Side side = Side::Left);

public:
    Side getSide();
    // CypressSession interface
public:
    void validate() const override;
    void calculateInputs() override;
    void start() override;
    void end() override;

private:
    Side m_side{Left};
};

#endif // RETINALCAMERASESSION_H
