#include "defaultdeletesessionrequesthandler.h"

#include "CypressApplication.h"
#include <string>
#include <iostream>
#include <fstream>

#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QDebug>

#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

#include "auxiliary/JsonSettings.h"

void DefaultDeleteSessionRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    std::string uri = request.getURI();

    response.setStatus(Poco::Net::HTTPResponse::HTTP_ACCEPTED);
    response.setContentType("application/json");

    bool success = CypressApplication::getInstance().forceSessionEnd();
    QJsonObject responseData {};
    if (success) {
       responseData["success"] = "Ended session";
    }
    else  {
       responseData["error"] = "Could not end session..";
    }

    std::ostream& out = response.send();

    out << JsonSettings::serializeJson(responseData).toStdString();
    out.flush();
}
