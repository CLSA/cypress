#ifndef GRIPSTRENGTHSESSION_H
#define GRIPSTRENGTHSESSION_H

#include "cypress_session.h"

class GripStrengthSession : public CypressSession
{
public:
    explicit GripStrengthSession(QObject *parent, const QJsonObject& inputData);

    // CypressSession interface
public:
    void initializeDialog() override;
    void isInstalled() const override;
    void isAvailable() const override;
};

#endif // GRIPSTRENGTHSESSION_H
