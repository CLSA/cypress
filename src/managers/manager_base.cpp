#include "manager_base.h"
#include "cypress_session.h"
#include "auxiliary/json_settings.h"

#include <sstream>

#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/AcceptCertificateHandler.h"
#include "Poco/StreamCopier.h"
#include "Poco/URI.h"

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

ManagerBase::ManagerBase(QSharedPointer<CypressSession> session)
    : m_session(session)
    , m_inputData(jsonObjectToVariantMap(m_session->getInputData()))
{
    m_debug = CypressSettings::isDebugMode();
    m_sim = CypressSettings::isSimMode();
}

ManagerBase::~ManagerBase()
{
}

QVariantMap ManagerBase::jsonObjectToVariantMap(const QJsonObject& jsonObject)
{
    // Convert QJsonObject to QJsonDocument
    QJsonDocument jsonDoc(jsonObject);

    // Convert QJsonDocument to QByteArray
    QByteArray byteArray = jsonDoc.toJson(QJsonDocument::Compact);

    // Convert QByteArray to QJsonDocument
    QJsonDocument convertedJsonDoc = QJsonDocument::fromJson(byteArray);

    // Convert QJsonDocument to QVariantMap
    QVariantMap variantMap = convertedJsonDoc.toVariant().toMap();

    return variantMap;
}

bool ManagerBase::sendCancellation(QString uuid)
{
    if (uuid.isEmpty() || uuid.isNull())
        throw std::exception("ManagerBase::sendCancellation - UUID parameter cannot be null or empty");

    QJsonObject data {
        { "status", "cancelled" }
    };

    QString host = CypressSettings::getPineHost();
    QString endpoint = CypressSettings::getDeviceEndpoint() + uuid;

    QByteArray serializedData = JsonSettings::serializeJson(data).toUtf8();

    sendHTTPSRequest("PATCH", host + endpoint, "application/json", serializedData);

    return true;
}

bool ManagerBase::sendComplete(QString uuid)
{
    if (uuid.isEmpty() || uuid.isNull())
        throw std::exception("ManagerBase::sendComplete - UUID parameter cannot be null or empty");

    QJsonObject data {
        { "status", "completed" }
    };

    QByteArray serializedData = JsonSettings::serializeJson(data).toUtf8();

    QString host = CypressSettings::getPineHost();
    QString endpoint = CypressSettings::getDeviceEndpoint() + uuid;

    sendHTTPSRequest("PATCH", host + endpoint, "application/json", serializedData);

    return true;
}

void ManagerBase::setManualEntry(bool isManualEntry)
{
    qDebug() << "set manual entry";
    if (!m_test.isNull()) {
        qDebug() << "not null";
        m_test->setManualEntryMode(isManualEntry);
    }
}

void ManagerBase::addManualMeasurement()
{
    return;
}

void ManagerBase::sendHTTPRequest(const QString& method, const QString& endpoint, const QString& contentType, const QByteArray& data)
{
    Q_UNUSED(data)
    Q_UNUSED(method)
    Q_UNUSED(endpoint)
    Q_UNUSED(contentType)
}

void ManagerBase::sendHTTPSRequest(const QString& method, const QString& endpoint, const QString& contentType, const QByteArray& data)
{
    Q_UNUSED(method)

    Poco::Net::initializeSSL();
    Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> ptrHandler = new Poco::Net::AcceptCertificateHandler(false);
    Poco::Net::Context::Ptr ptrContext = new Poco::Net::Context(
        Poco::Net::Context::CLIENT_USE, "", "", "",
        Poco::Net::Context::VERIFY_RELAXED, 9, true,
        "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"
    );
    Poco::Net::SSLManager::instance().initializeClient(0, ptrHandler, ptrContext);

    const QString &pinePath = QString(endpoint);

    Poco::URI uri(pinePath.toStdString());
    Poco::Net::HTTPSClientSession session(uri.getHost(), uri.getPort());

    std::string path(uri.getPathAndQuery());
    if (path.empty()) path = "/";

    Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_PATCH, path, Poco::Net::HTTPMessage::HTTP_1_1);

    QString credentials = CypressSettings::getPineCredentials();

    req.setContentType(contentType.toUtf8().toStdString());
    req.setContentLength(data.length());
    req.setCredentials("Basic", credentials.toUtf8().toBase64().toStdString());

    std::ostream &os = session.sendRequest(req);
    os.write(data.constData(), data.size());
    os.flush();

    Poco::Net::HTTPResponse res;

    std::istream &is = session.receiveResponse(res);
    std::stringstream ss;
    Poco::StreamCopier::copyStream(is, ss);

    switch (res.getStatus())
    {
    case Poco::Net::HTTPResponse::HTTP_BAD_REQUEST:
        QMessageBox::critical(nullptr, "Bad Request", "Please contact support");
        break;
    case Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR:
        QMessageBox::critical(nullptr, "Internal Server Error", "Please contact support");
        break;
    case Poco::Net::HTTPResponse::HTTP_FORBIDDEN:
        QMessageBox::critical(nullptr, "Forbidden", "Please contact support");
        break;
    case Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED:
        QMessageBox::critical(nullptr, "Unauthorized", "Please contact support");
        break;
    case Poco::Net::HTTPResponse::HTTP_GATEWAY_TIMEOUT:
        QMessageBox::critical(nullptr, "Gateway Timeout", "Please contact support");
        break;
    default:
        QMessageBox::critical(nullptr, "HTTP Error", QString::fromStdString(res.getReason()));
        break;
    }
}
