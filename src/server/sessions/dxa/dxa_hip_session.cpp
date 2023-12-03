#include "dxa_hip_session.h"
#include "dialogs/dxa_hip_dialog.h"

DxaHipSession::DxaHipSession(QObject *parent, const QJsonObject& inputData)
    : CypressSession{parent, inputData}
{

}

void DxaHipSession::start()
{
    m_dialog = new DxaHipDialog(nullptr, QSharedPointer<DxaHipSession>(this));
    if (m_dialog == nullptr)
        throw QException();

    m_startDateTime = QDateTime::currentDateTimeUtc();
    m_status = SessionStatus::Started;

    m_dialog->run();
    m_dialog->show();

    if (m_debug)
        qDebug() << "DxaHipSession::start " << getSessionId() << m_startDateTime;
}
