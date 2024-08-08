#ifndef JTECH_SESSION_H
#define JTECH_SESSION_H

#include "cypress_session.h"

class JTechSession : public CypressSession
{
public:
    JTechSession(QObject *parent, const QJsonObject &inputData, const QString &origin);

    // CypressSession interface
public:
    void initializeDialog();
    void isInstalled() const;
    void isAvailable() const;
    void validate() const;
    void calculateInputs();
};

#endif // JTECH_SESSION_H
