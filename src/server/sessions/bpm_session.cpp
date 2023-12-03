#include "bpm_session.h"
#include "dialogs/blood_pressure_dialog.h"

BPMSession::BPMSession(QObject *parent, const QJsonObject& inputData)
    : CypressSession{parent, inputData}
{

}

void BPMSession::start()
{
    m_dialog = new BloodPressureDialog(nullptr, QSharedPointer<BPMSession>(this));
    if (m_dialog == nullptr)
        throw QException();

    m_startDateTime = QDateTime::currentDateTimeUtc();
    m_status = SessionStatus::Started;

    m_dialog->run();
    m_dialog->show();

    if (m_debug)
        qDebug() << "BPMSession::start" << getSessionId() << m_startDateTime;
}
