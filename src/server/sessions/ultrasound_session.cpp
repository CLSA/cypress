#include "ultrasound_session.h"

#include "dialogs/cimt_vivid_i_dialog.h"

UltrasoundSession::UltrasoundSession(QObject *parent, const QJsonObject& inputData)
    : CypressSession{parent, inputData}
{
    qDebug() << "create ultrsound session";
}

UltrasoundSession::~UltrasoundSession()
{
    qDebug() << "delete session";
}

void UltrasoundSession::validate() const
{
    CypressSession::validate();
    qDebug() << "validate ultrsound session";
}

void UltrasoundSession::start()
{
    m_dialog = new CimtVividiDialog(nullptr, QSharedPointer<UltrasoundSession>(this));
    if (m_dialog == nullptr)
        throw QException();

    m_startDateTime = QDateTime::currentDateTimeUtc();
    m_status = SessionStatus::Started;

    m_dialog->run();
    m_dialog->show();

    if (m_debug)
        qDebug() << "UltrasoundSession::start " << getSessionId() << m_startDateTime;
}
