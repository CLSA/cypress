#include "cypress_session.h"

#include "dialogs/dialog_factory.h"
#include "auxiliary/Constants.h"
#include "managers/participant_report/participant_report_manager.h"

#include <QException>



CypressSession::CypressSession():
    m_deviceType(Constants::MeasureType::Unknown),
    m_inputData(QJsonObject {})
{

}

CypressSession::CypressSession(const Constants::MeasureType& device, const QJsonObject& inputData):
    m_deviceType(device),
    m_inputData(inputData)
{
    qDebug() << "CypressSession::DeviceSession" << inputData;

    validate(inputData);

    m_barcode = m_inputData.value("barcode").toString();
    m_answerId = m_inputData.value("answer_id").toInt();
    m_interviewer = m_inputData.value("interviewer").toString();
    m_language = m_inputData.value("language").toString();
    m_sessionId = QUuid::createUuid().toString(QUuid::WithoutBraces);
    m_startDateTime = QDateTime::currentDateTimeUtc();
};

CypressSession::CypressSession(const Constants::ReportType& report, const QJsonObject& inputData):
    m_reportType(report),
    m_inputData(inputData)
{
    qDebug() << "CypressSession::ReportSession" << inputData;

    validate(inputData);

    m_inputData = inputData;
    m_barcode = m_inputData.value("barcode").toString();
    m_answerId = m_inputData.value("answer_id").toInt();
    m_interviewer = m_inputData.value("interviewer").toString();
    m_language = m_inputData.value("language").toString();
    m_sessionId = QUuid::createUuid().toString(QUuid::WithoutBraces);
};


CypressSession::CypressSession(const CypressSession& session):
    m_deviceType(session.getDeviceType()),
    m_reportType(session.getReportType()),
    m_inputData(session.getInputData())
{
    qDebug() << "CypressSession::CopySession";

    m_inputData = session.getInputData();
    m_barcode = session.getBarcode();
    m_answerId = session.getAnswerId();
    m_interviewer = session.getInterviewer();
    m_language = session.getLanguage();
    m_sessionId = session.getSessionId();
    m_startDateTime = session.m_startDateTime;
    m_endDateTime = session.m_endDateTime;
}

CypressSession::~CypressSession()
{
    qDebug() << "CypressSession::Destroyed";
}

void CypressSession::validate(const QJsonObject& inputData) const
{
    if (inputData.isEmpty())
        throw InvalidInputDataException();

    if (!isValidString(inputData, "barcode"))
        throw InvalidBarcodeException();

    if (!isValidInteger(inputData, "answer_id"))
        throw InvalidAnswerIdException();

    if (!isValidString(inputData, "language"))
        throw InvalidLanguageException();

    if (!isValidString(inputData, "interviewer"))
        throw InvalidInterviewerException();
}

void CypressSession::start()
{
    m_startDateTime = QDateTime::currentDateTimeUtc();
    m_status = SessionStatus::Started;

    m_deviceDialog.reset(DialogFactory::instantiate(*this));
    m_deviceDialog->run();
    m_deviceDialog->show();
}

void CypressSession::end()
{
    if (m_deviceDialog.isNull())
        throw QException();

    m_status = SessionStatus::Ended;
    m_endDateTime = QDateTime::currentDateTimeUtc();

    qDebug() << "end session" << getSessionId() << m_endDateTime;
}

void CypressSession::startReport()
{
    if (m_reportType == Constants::ReportType::ParticipantReportEn)
    {
        ParticipantReportManager participantManager(*this);

        participantManager.start();
        participantManager.measure();
        participantManager.finish();
    }
}

void CypressSession::endReport()
{

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
    QJsonObject obj;

    obj.insert("answer_id", getAnswerId());
    obj.insert("session_id", getSessionId());
    obj.insert("interviewer", getInterviewer());
    obj.insert("start_time", getStartTime().toString("yyyy-MM-ddThh:mm:ss.zzzZ"));
    obj.insert("end_time", QDateTime::currentDateTimeUtc().toString("yyyy-MM-ddThh:mm:ss.zzzZ"));
    obj.insert("barcode", getBarcode());
    obj.insert("language", getLanguage());

    return obj;
}

Constants::MeasureType CypressSession::getDeviceType() const
{
    return m_deviceType;
}

Constants::ReportType CypressSession::getReportType() const
{
    return m_reportType;
}

SessionStatus CypressSession::getStatus() const
{
    return m_status;
}

QJsonObject CypressSession::getInputData() const
{
    return m_inputData;
}


bool CypressSession::isValidString(const QJsonObject& inputData, const QString& key) const
{
    if (!inputData.contains(key)) return false;

    QJsonValue val { inputData.value(key) };

    if (!val.isString())      return false;
    if (val.toString() == "") return false;

    return true;
}


bool CypressSession::isValidDouble(const QJsonObject& inputData, const QString& key) const
{
    if (!inputData.contains(key)) return false;

    QJsonValue val { inputData.value(key) };
    if (val.isDouble()) return true;

    return false;
}


bool CypressSession::isValidInteger(const QJsonObject& inputData, const QString& key) const
{
    if (!inputData.contains(key)) return false;

    QJsonValue val { inputData.value(key) };
    if (val.isDouble()) return true;

    return false;
}





