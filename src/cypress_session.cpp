#include "cypress_session.h"
#include <QException>

CypressSession::CypressSession(QJsonObject inputData)
{
    qDebug() << "CypressSession::Constructor" << m_inputData;
    if (!isValid(inputData))
        throw QException();
};

CypressSession::~CypressSession()
{
    qDebug() << "CypressSession::Destructor";
}



bool CypressSession::isValid(const QJsonObject& inputData)
{
    if (inputData.isEmpty())
        return false;

    if (!inputData.contains("barcode") || inputData.value("barcode").isNull() || inputData.value("barcode").isUndefined() || inputData.value("barcode").isString())
        return false;

    if (!inputData.contains("answer_id"))
        return false;

    if (!inputData.contains("language"))
        return false;

    if (!inputData.contains("interviewer"))
        return false;

    return true;
}

QString CypressSession::getSessionId()
{
    return m_inputData.value("session_id").;
}

QString CypressSession::getBarcode()
{
    return "";
}

QString CypressSession::getLanguage()
{
    return "";
}

QString CypressSession::getInterviewer()
{
    return "";
}
