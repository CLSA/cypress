#ifndef RETINALCAMERASESSION_H
#define RETINALCAMERASESSION_H

#include "cypress_session.h"


class RetinalCameraSession : public CypressSession
{
public:
    enum Side { Left, Right };
    explicit RetinalCameraSession(QObject *parent,
                                  const QJsonObject &inputData,
                                  const QString& origin,
                                  Side side = Side::Left);

public:
    Side getSide();

    // CypressSession interface
public:
    void initializeDialog() override;
    void isInstalled() const override;
    void isAvailable() const override;

private:
    Side m_side{Left};
};

#endif // RETINALCAMERASESSION_H
