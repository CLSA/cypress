#include "cypress_session.h"

//#include "dialogs/dialog_factory.h"
//#include "auxiliary/Constants.h"
//#include "managers/participant_report/participant_report_manager.h"

#include <QException>


CypressSession::CypressSession(QObject* parent, const QJsonObject& inputData):
    QObject(parent), m_inputData(inputData)
{
    m_barcode = m_inputData.value("barcode").toString();
    m_answerId = m_inputData.value("answer_id").toInt();
    m_interviewer = m_inputData.value("interviewer").toString();
    m_language = m_inputData.value("language").toString();
    m_sessionId = QUuid::createUuid().toString(QUuid::WithoutBraces);
    m_startDateTime = QDateTime::currentDateTimeUtc();

}

void CypressSession::validate() const
{
    if (m_inputData.isEmpty())
    {
        throw ValidationError("empty");
    }

    if (!isValidString("barcode"))
    {
        throw ValidationError("barcode");
    }

    if (!isValidInteger("answer_id"))
    {
        throw ValidationError("answer_id");
    }

    if (!isValidString("language"))
    {
        throw ValidationError("language");
    }

    if (!isValidString("interviewer"))
    {
        throw ValidationError("interviewer");
    }
}

void CypressSession::start()
{
    if (m_dialog.isNull())
        throw QException();

    m_startDateTime = QDateTime::currentDateTimeUtc();
    m_status = SessionStatus::Started;

    m_dialog->run();
    m_dialog->show();

    qDebug() << "start session" << getSessionId() << m_startDateTime;
}

void CypressSession::end()
{
    if (m_dialog.isNull())
        throw QException();

    m_status = SessionStatus::Ended;
    m_endDateTime = QDateTime::currentDateTimeUtc();

    qDebug() << "end session" << getSessionId() << m_endDateTime;
}


QString CypressSession::getSessionId() const
{
    return m_sessionId;
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


SessionStatus CypressSession::getStatus() const
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





