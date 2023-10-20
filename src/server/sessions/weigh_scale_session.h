#ifndef WEIGHSCALESESSION_H
#define WEIGHSCALESESSION_H

#include "cypress_session.h"

#include <QJsonObject>

class WeighScaleSession : public CypressSession
{
public:
    explicit WeighScaleSession(QObject *parent, const QJsonObject& inputData);

    // CypressSession interface
public:
    void validate() const override;
    void calculateInputs() override;
    void start() override;
    void end() override;
};

#endif // WEIGHSCALESESSION_H
