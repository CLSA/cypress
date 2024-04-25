#ifndef AUDIOMETERSESSION_H
#define AUDIOMETERSESSION_H

#include "cypress_session.h"

class AudiometerSession : public CypressSession
{
public:
    explicit AudiometerSession(QObject *parent, const QJsonObject& inputData, const QString& origin);

public:
    void initializeDialog() override;
    void isInstalled() const override;
    void isAvailable() const override;
};

#endif // AUDIOMETERSESSION_H
