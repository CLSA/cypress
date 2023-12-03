#ifndef ECGSESSION_H
#define ECGSESSION_H

#include "cypress_session.h"

class ECGSession : public CypressSession
{
public:
    explicit ECGSession(QObject *parent, const QJsonObject& inputData);

    // CypressSession interface
public:
    void start() override;
};

#endif // ECGSESSION_H
