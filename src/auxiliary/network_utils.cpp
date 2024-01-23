#include "network_utils.h"

#include <sstream>

#include <QDebug>
#include <QException>

#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/AcceptCertificateHandler.h"
#include "Poco/StreamCopier.h"
#include "Poco/URI.h"

#include "cypress_settings.h"

bool NetworkUtils::sendHTTPRequest(
    const std::string &method,
    const std::string &endpoint,
    const std::string &contentType,
    const QByteArray& data)
{
    Q_UNUSED(data)
    Q_UNUSED(method)
    Q_UNUSED(endpoint)
    Q_UNUSED(contentType)

    return false;
}

bool NetworkUtils::sendHTTPSRequest(
    const std::string &method,
    const std::string &endpoint,
    const std::string &contentType,
    const QByteArray &data)
{
    try {
        Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> ptrHandler
            = new Poco::Net::AcceptCertificateHandler(false);

        Poco::Net::Context::Ptr ptrContext
            = new Poco::Net::Context(
                Poco::Net::Context::CLIENT_USE, "", "", "",
                Poco::Net::Context::VERIFY_NONE, 9, true);

        Poco::Net::SSLManager::instance().initializeClient(0, ptrHandler, ptrContext);

        Poco::URI uri(endpoint);
        Poco::Net::HTTPSClientSession session(uri.getHost(), uri.getPort());

        std::string path(uri.getPathAndQuery());
        if (path.empty())
            path = "/";

        Poco::Net::HTTPRequest req(method,
                                   path,
                                   Poco::Net::HTTPMessage::HTTP_1_1);

        QString credentials = CypressSettings::getPineCredentials();
        req.setContentType(contentType);
        req.setContentLength(data.length());
        req.setCredentials("Basic", credentials.toUtf8().toBase64().toStdString());

        std::ostream &os = session.sendRequest(req);
        os.write(data.constData(), data.size());
        os.flush();

        Poco::Net::HTTPResponse res;

        std::istream &is = session.receiveResponse(res);
        std::stringstream ss;
        Poco::StreamCopier::copyStream(is, ss);

        qDebug() << res.getStatus() << ss.str().c_str();
        switch (res.getStatus()) {
        case Poco::Net::HTTPResponse::HTTP_OK:
            return true;
        case Poco::Net::HTTPResponse::HTTP_BAD_REQUEST:
            return false;
        case Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR:
            return false;
        case Poco::Net::HTTPResponse::HTTP_FORBIDDEN:
            return false;
        case Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED:
            return false;
        case Poco::Net::HTTPResponse::HTTP_GATEWAY_TIMEOUT:
            return false;
        case Poco::Net::HTTPResponse::HTTP_NOT_FOUND:
            return false;
        default:
            return false;
        }
    }
    catch (const Poco::Exception &e) {
        qDebug() << e.what() << e.message().c_str();
        return false;
    }
    catch (const QException &e) {
        qDebug() << e.what();
        return false;
    }
    catch (const std::exception &e) {
        qDebug() << e.what();
        return false;
    }

    return true;
}
