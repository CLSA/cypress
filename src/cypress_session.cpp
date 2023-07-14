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
    m_answerId = m_inputData.value("answer_id").toString();
    m_interviewer = m_inputData.value("interviewer").toString();
    m_language = m_inputData.value("language").toString();
    m_sessionId = QUuid::createUuid().toString(QUuid::WithoutBraces);
};

CypressSession::CypressSession(const Constants::ReportType& report, const QJsonObject& inputData):
    m_reportType(report),
    m_inputData(inputData)
{
    qDebug() << "CypressSession::ReportSession" << inputData;

    validate(inputData);

    m_inputData = inputData;
    m_barcode = m_inputData.value("barcode").toString();
    m_answerId = m_inputData.value("answer_id").toString();
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

    if (!isValidString(inputData, "answer_id"))
        throw InvalidAnswerIdException();

    if (!isValidString(inputData, "language"))
        throw InvalidLanguageException();

    if (!isValidString(inputData, "interviewer"))
        throw InvalidInterviewerException();
}

void CypressSession::startDevice()
{
    qDebug() << "CypressSession::start()";

    if (!m_deviceDialog.isNull())
        throw QException();

    m_deviceDialog.reset(DialogFactory::instantiate(*this) /* pass a const reference of the session to the dialog */ );
    m_deviceDialog->run();
    m_deviceDialog->show();
}

void CypressSession::endDevice()
{
    if (m_deviceDialog.isNull())
        throw QException();

    qDebug() << "end session" << getSessionId();
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

QString CypressSession::getAnswerId() const
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

Constants::MeasureType CypressSession::getDeviceType() const
{
    return m_deviceType;
}

Constants::ReportType CypressSession::getReportType() const
{
    return m_reportType;
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





