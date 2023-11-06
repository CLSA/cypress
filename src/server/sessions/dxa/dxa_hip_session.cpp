#include "dxa_hip_session.h"
#include "dialogs/dxa_hip_dialog.h"

DxaHipSession::DxaHipSession(QObject *parent, const QJsonObject& inputData)
    : CypressSession{parent, inputData}
{

}

void DxaHipSession::validate() const
{
    CypressSession::validate();
}

void DxaHipSession::calculateInputs()
{
    CypressSession::calculateInputs();
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

    qDebug() << "start session" << getSessionId() << m_startDateTime;
}

void DxaHipSession::end()
{

}
