#include "cypress_session.h"

#include <QException>

#include "auxiliary/validators.h"


CypressSession::CypressSession(QObject* parent, const QJsonObject& inputData, const QString& origin):
    QObject(parent), m_inputData(inputData)
{
    m_barcode = m_inputData.value("barcode").toString();
    m_uid = m_inputData.value("uid").toString();
    m_answerId = m_inputData.value("answer_id").toInt();
    m_interviewer = m_inputData.value("interviewer").toString();
    m_language = m_inputData.value("language").toString();
    m_sessionId = QUuid::createUuid().toString(QUuid::WithoutBraces);
    m_startDateTime = QDateTime::currentDateTimeUtc();
    m_origin = origin;
    m_debug = CypressSettings::isDebugMode();
}

QString CypressSession::getOrigin() const {
    return m_origin;
}

void CypressSession::validate() const
{
    if (m_inputData.isEmpty())
        throw ValidationError("empty");
    if (m_origin.isEmpty())
        throw ValidationError("origin header");
    if (!isValidString("barcode"))
        throw ValidationError("barcode");
    if (!isValidInteger("answer_id"))
        throw ValidationError("answer_id");
    if (!isValidString("language"))
        throw ValidationError("language");
    if (!isValidString("interviewer"))
        throw ValidationError("interviewer");
}

void CypressSession::start()
{
    qDebug() << "start";
    if (m_dialog == nullptr)
        throw QException();
    qDebug() << "dialog isn't null";

    m_startDateTime = QDateTime::currentDateTimeUtc();
    m_status = SessionStatus::Started;

    if (m_dialog->run()) {
        m_dialog->show();
        if (CypressSettings::isDebugMode())
            qDebug() << "start session" << getSessionId() << m_startDateTime;
    }
    else {
        end(SessionStatus::CriticalError);
    }
}

void CypressSession::end(const CypressSession::SessionStatus& status)
{
    if (m_dialog == nullptr)
        return;

    m_status = status;
    m_endDateTime = QDateTime::currentDateTimeUtc();

    m_dialog->close();

    if (CypressSettings::isDebugMode())
        qDebug() << "end session" << getSessionId() << m_endDateTime;
}


QString CypressSession::getSessionId() const
{
    return m_sessionId;
}

QString CypressSession::getUID() const
{
    return m_uid;
}

int CypressSession::getAnswerId() const
{
    return m_answerId;
}

QString CypressSession::getBarcode() const
{
    return m_barcode;
}

QString CypressSession::getLanguage() const
{
    return m_language;
}

QString CypressSession::getInterviewer() const
{
    return m_interviewer;
}

QDateTime CypressSession::getStartTime() const
{
    return m_startDateTime;
}

QDateTime CypressSession::getEndTime() const
{
    return m_endDateTime;
}

QJsonObject CypressSession::getJsonObject() const
{
    QJsonObject obj = m_inputData;

    obj.insert("answer_id", 	getAnswerId());
    obj.insert("session_id", 	getSessionId());
    obj.insert("interviewer", 	getInterviewer());
    obj.insert("start_time", 	getStartTime().toString("yyyy-MM-ddThh:mm:ss.zzzZ"));
    obj.insert("end_time", 		QDateTime::currentDateTimeUtc().toString("yyyy-MM-ddThh:mm:ss.zzzZ"));
    obj.insert("barcode", 		getBarcode());
    obj.insert("language", 		getLanguage());

    return obj;
}


CypressSession::SessionStatus CypressSession::getStatus() const
{
    return m_status;
}

QJsonObject CypressSession::getInputData() const
{
    return m_inputData;
}


bool CypressSession::isValidString(const QString& key) const
{
    if (!m_inputData.contains(key)) return false;

    QJsonValue val { m_inputData.value(key) };

    if (!val.isString())      return false;
    if (val.toString() == "") return false;

    return true;
}


bool CypressSession::isValidDouble(const QString& key) const
{
    if (!m_inputData.contains(key)) return false;

    QJsonValue val { m_inputData.value(key) };
    if (val.isDouble()) return true;

    return false;
}


bool CypressSession::isValidInteger(const QString& key) const
{
    if (!m_inputData.contains(key)) return false;

    QJsonValue val { m_inputData.value(key) };
    if (val.isDouble()) return true;

    return false;
}


bool CypressSession::isValidBool(const QString& key) const
{
    if (!m_inputData.contains(key)) return false;

    QJsonValue val { m_inputData.value(key) };

    if (val.isBool()) return true;

    return false;
}


bool CypressSession::isValidDate(const QString& key, const QString& dateFormat) const
{

    if (!m_inputData.contains(key)) return false;

    QJsonValue val { m_inputData.value(key) };

    if (!val.isString()) return false;

    QDate date = QDate::fromString(val.toString(), dateFormat);
    qDebug() << date;

    return date.isValid();
}


bool CypressSession::isValidDateTime(const QString& key, const QString& dateFormat) const
{
    if (!m_inputData.contains(key)) return false;

    QJsonValue val { m_inputData.value(key) };

    if (!val.isString()) return false;

    QDate date = QDate::fromString(val.toString(), dateFormat);

    return date.isValid();
}





