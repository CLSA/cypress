#include "utils.h"
#include "auxiliary/json_settings.h"
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

//QJsonObject Utils::httpPost(const QJsonObject& data)
//{
//    try
//    {
//        QString dataString = JsonSettings::serializeJson(data);
//
//        URI uri("http://localhost:8000/");
//        HTTPClientSession session(uri.getHost(), uri.getPort());
//
//        std::string path(uri.getPathAndQuery());
//        if (path.empty()) path = "/";
//
//        HTTPRequest req(HTTPRequest::HTTP_PATCH, path, HTTPMessage::HTTP_1_1);
//
//        req.setContentType("application/json");
//        req.setContentLength(dataString.length());
//
//        std::ostream &os = session.sendRequest(req);
//        os << dataString.toStdString();
//
//        HTTPResponse res;
//
//        std::istream &is = session.receiveResponse(res);
//        std::stringstream ss;
//        StreamCopier::copyStream(is, ss);
//    }
//    catch (Exception &e)
//    {
//        qDebug() << e.what();
//    }
//
//    return QJsonObject();
//}
