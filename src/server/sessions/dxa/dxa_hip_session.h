#ifndef DXAHIPSESSION_H
#define DXAHIPSESSION_H

#include "cypress_session.h"

class DxaHipSession : public CypressSession
{
public:
    explicit DxaHipSession(QObject *parent, const QJsonObject& inputData);

    // CypressSession interface
public:
    QString getWebpageContents() const override;

    void validate() const override;
    void initializeDialog() override;
    void isInstalled() const override;
    void isAvailable() const override;
};

#endif // DXAHIPSESSION_H
