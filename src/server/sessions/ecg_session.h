#ifndef ECGSESSION_H
#define ECGSESSION_H

#include "cypress_session.h"

class ECGSession : public CypressSession
{
public:
    explicit ECGSession(QObject *parent, const QJsonObject& inputData, const QString& origin);

    // CypressSession interface
public:
    void isInstalled() const override;
    void isAvailable() const override;
    void initializeDialog() override;
};

#endif // ECGSESSION_H
