#ifndef PARTICIPANTREPORTMANAGER_H
#define PARTICIPANTREPORTMANAGER_H

#include "../manager_base.h"
#include "server/sessions/participant_report_session.h"

class ParticipantReportManager : public ManagerBase
{
public:
    ParticipantReportManager(QSharedPointer<ParticipantReportSession> session);

    // ManagerBase interface
public slots:
    void start();
    void measure();
    void finish();

protected:
    void setInputData(const QVariantMap &);
    bool setUp();
    bool clearData();
    bool cleanUp();
};

#endif // PARTICIPANTREPORTMANAGER_H
