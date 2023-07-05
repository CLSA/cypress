#include "cypress_session.h"

#include "dialogs/dialog_factory.h"
#include "auxiliary/Constants.h"

#include <QException>

CypressSession::CypressSession(const Constants::MeasureType& device, const QJsonObject& inputData): m_deviceType(device), m_inputData(inputData)
{
    qDebug() << "CypressSession::Constructor" << inputData;
    validate(inputData);

    m_barcode = m_inputData.value("barcode").toString();
    m_answerId = m_inputData.value("answer_id").toString();
    m_interviewer = m_inputData.value("interviewer").toString();
    m_language = m_inputData.value("language").toString();
    m_sessionId = QUuid::createUuid().toString(QUuid::WithoutBraces);
};


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

bool CypressSession::start()
{
    if (!m_deviceDialog.isNull())
        throw QException();

    m_deviceDialog.reset(DialogFactory::instantiate(*this) /* pass a const reference of the session to the dialog */ );
    m_deviceDialog->initialize();
    m_deviceDialog->m_manager->m_uuid = getSessionId();
    m_deviceDialog->show();

    return true;
}

bool CypressSession::end()
{
    if (m_deviceDialog.isNull())
        throw QException();

    m_deviceDialog->userClose();

    return true;
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





