#ifndef SPIROMETERSESSION_H
#define SPIROMETERSESSION_H

#include "cypress_session.h"

class SpirometerSession : public CypressSession
{
public:
    explicit SpirometerSession(QObject *parent, const QJsonObject& inputData);

    // CypressSession interface
public:
    void isInstalled() const override;
    void isAvailable() const override;
    void validate() const override;

    void calculateInputs() override;

    void initializeDialog() override;
private:
    int computeAge(const QString &birthdateStr);
};

#endif // SPIROMETERSESSION_H
