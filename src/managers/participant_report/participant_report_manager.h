#ifndef PARTICIPANTREPORTMANAGER_H
#define PARTICIPANTREPORTMANAGER_H

#include "managers/manager_base.h"

#include <QObject>

class ParticipantReportManager : public ManagerBase
{
    Q_OBJECT
public:
    ParticipantReportManager(QSharedPointer<CypressSession> session);

    // ManagerBase interface
public slots:
    void start() override;
    void measure() override;
    void finish() override;

protected:
    void setInputData(const QVariantMap &) override;
    bool setUp() override;
    bool clearData() override;
    bool cleanUp() override;
};

#endif // PARTICIPANTREPORTMANAGER_H
