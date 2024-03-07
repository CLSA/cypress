#ifndef DXASESSION_H
#define DXASESSION_H

#include "cypress_session.h"

class DXASession : public CypressSession
{
public:
    explicit DXASession(QObject *parent, const QJsonObject& inputData);

    // CypressSession interface
public:
    QString getWebpageContents() const override;

    void validate() const override;
    void initializeDialog() override;
    void isInstalled() const override;
    void isAvailable() const override;
};

#endif // DXASESSION_H
