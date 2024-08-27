#ifndef VIVID_IQ_SESSION_H
#define VIVID_IQ_SESSION_H

#include "cypress_session.h"

class VividIQSession : public CypressSession
{
public:
    VividIQSession(QObject *parent, const QJsonObject &inputData, const QString &origin);

    // CypressSession interface
public:
    void initializeDialog();
    void isInstalled() const;
    void isAvailable() const;
};

#endif // VIVID_IQ_SESSION_H
