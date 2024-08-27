#ifndef MAC5_SESSION_H
#define MAC5_SESSION_H

#include "cypress_session.h"

class Mac5Session : public CypressSession
{
public:
    Mac5Session(QObject *parent, const QJsonObject &inputData, const QString &origin);

    // CypressSession interface
public:
    void initializeDialog();
    void isInstalled() const;
    void isAvailable() const;
};

#endif // MAC5_SESSION_H
