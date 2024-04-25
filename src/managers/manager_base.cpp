#include "manager_base.h"

#include "Poco/Net/HTTPRequest.h"

#include <QStandardItemModel>
#include <QException>
#include <QDebug>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariantMap>
#include <QMessageBox>

#include "cypress_session.h"

#include "auxiliary/json_settings.h"
#include "auxiliary/network_utils.h"

ManagerBase::ManagerBase(QSharedPointer<CypressSession> session)
    : m_session(session)
    , m_inputData(JsonSettings::jsonObjectToVariantMap(m_session->getInputData()))
{
    m_debug = CypressSettings::isDebugMode();
    m_sim = CypressSettings::isSimMode();
}

ManagerBase::~ManagerBase()
{
}

void ManagerBase::finish() {
    qDebug() << "ManagerBase::finish";

    int answer_id = m_session->getAnswerId();
    const QString pineOrigin = m_session->getOrigin();
    const QString answerUrl = pineOrigin + "/answer/" + QString::number(answer_id);
    qDebug() <<  answerUrl;

    QJsonObject testJson = m_test->toJsonObject();
    testJson.insert("session", m_session->getJsonObject());

    QJsonObject responseJson {};
    responseJson.insert("value", testJson);

    QJsonDocument jsonDoc(responseJson);
    QByteArray serializedData = jsonDoc.toJson();

    bool ok = NetworkUtils::sendHTTPSRequest(
        Poco::Net::HTTPRequest::HTTP_PATCH,
        answerUrl.toStdString(),
        "application/json",
        serializedData
    );

    cleanUp();

    if (ok)
        emit success("Save successful. You may close this window.");
    else
        emit error("Something went wrong");
}

bool ManagerBase::sendCancellation(QString uuid) {
    qDebug() << "ManagerBase::sendCancellation";

    const QJsonObject data {
        { "status", "cancelled" }
    };

    const QString pineOrigin = m_session->getOrigin();
    const QString answerUrl = pineOrigin + "/answer_device/uuid=" + uuid;
    const QByteArray serializedData = JsonSettings::serializeJson(data).toUtf8();

    return NetworkUtils::sendHTTPSRequest(
        Poco::Net::HTTPRequest::HTTP_PATCH,
        answerUrl.toStdString(),
        "application/json",
        serializedData
    );
}

bool ManagerBase::sendComplete(QString uuid)
{
    const QJsonObject data {
        { "status", "completed" }
    };

    const QString pineOrigin = m_session->getOrigin();
    const QString answerUrl = pineOrigin + "/answer_device/uuid=" + uuid;

    qDebug() << "sendComplete:" << answerUrl;

    const QByteArray serializedData = JsonSettings::serializeJson(data).toUtf8();

    return NetworkUtils::sendHTTPSRequest(
        Poco::Net::HTTPRequest::HTTP_PATCH,
        answerUrl.toStdString(),
        "application/json",
        serializedData
    );
}

void ManagerBase::readOutput() {

}

void ManagerBase::setManualEntry(bool isManualEntry)
{
    if (!m_test.isNull()) {
        m_test->setManualEntryMode(isManualEntry);
    }
}

void ManagerBase::addManualMeasurement()
{
    return;
}
