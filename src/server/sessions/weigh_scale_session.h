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
    void initializeDialog() override;
    void isInstalled() const override;
    void isAvailable() const override;
};

#endif // WEIGHSCALESESSION_H
