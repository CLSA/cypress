#ifndef ULTRASOUNDSESSION_H
#define ULTRASOUNDSESSION_H

#include "cypress_session.h"

#include <QJsonObject>

class UltrasoundSession : public CypressSession
{
public:
    explicit UltrasoundSession(QObject *parent, const QJsonObject& inputData, const QString& origin);

    // CypressSession interface
public:
    void initializeDialog() override;
    void isInstalled() const override;
    void isAvailable() const override;

};

#endif // ULTRASOUNDSESSION_H
