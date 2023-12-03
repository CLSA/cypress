#ifndef DXAHIPSESSION_H
#define DXAHIPSESSION_H

#include "cypress_session.h"

class DxaHipSession : public CypressSession
{
public:
    explicit DxaHipSession(QObject *parent, const QJsonObject& inputData);

    // CypressSession interface
public:
    void start();
};

#endif // DXAHIPSESSION_H
