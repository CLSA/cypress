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

void WeighScaleSession::calculateInputs()
{

}

void WeighScaleSession::start()
{
    m_dialog.reset(new WeighScaleDialog(nullptr, QSharedPointer<WeighScaleSession>(this)));
    if (m_dialog.isNull())
        throw QException();

    m_startDateTime = QDateTime::currentDateTimeUtc();
    m_status = SessionStatus::Started;

    m_dialog->run();
    m_dialog->show();

    qDebug() << "start session" << getSessionId() << m_startDateTime;
}

void WeighScaleSession::end()
{

}
