#include "utils.h"
#include "auxiliary/JsonSettings.h"
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/Path.h>
#include <Poco/URI.h>
#include <Poco/Exception.h>
#include <sstream>

using namespace Poco::Net;
using namespace Poco;

Utils::Utils()
{

}

QJsonObject Utils::httpPost(const QJsonObject& data)
{
    try
    {
        QString dataString = JsonSettings::serializeJson(data);

        URI uri("http://localhost:9000/");
        HTTPClientSession session(uri.getHost(), uri.getPort());

        std::string path(uri.getPathAndQuery());
        if (path.empty()) path = "/";

        HTTPRequest req(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
        req.setContentType("application/json");

        req.setContentLength(dataString.length());

        std::ostream &os = session.sendRequest(req);
        os << dataString.toStdString();

        qDebug() << "sent";
        HTTPResponse res;
        qDebug() << res.getStatus() << " " << QString::fromStdString(res.getReason());

        std::istream &is = session.receiveResponse(res);
        std::stringstream ss;
        StreamCopier::copyStream(is, ss);

        qDebug() << QString::fromStdString(ss.str());
    }
    catch (Exception &e)
    {

    }

    return QJsonObject();
}
