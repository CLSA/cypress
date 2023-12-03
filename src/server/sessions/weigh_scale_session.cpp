#include "weigh_scale_session.h"

#include "dialogs/weigh_scale_dialog.h"

WeighScaleSession::WeighScaleSession(QObject *parent, const QJsonObject& inputData)
    : CypressSession{parent, inputData}
{

}

void WeighScaleSession::validate() const
{
    CypressSession::validate();
}

void WeighScaleSession::start()
{
    m_dialog = new WeighScaleDialog(nullptr, QSharedPointer<WeighScaleSession>(this));
    if (m_dialog == nullptr)
        throw QException();

    m_startDateTime = QDateTime::currentDateTimeUtc();
    m_status = SessionStatus::Started;

    m_dialog->run();
    m_dialog->show();

    if (m_debug)
        qDebug() << "WeighScaleSession::start " << getSessionId() << m_startDateTime;
}
