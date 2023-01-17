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

#include "DefaultRequestHandler.h"
#include "auxiliary/JsonSettings.h"

void DefaultRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    std::string uri = request.getURI();

    response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
    response.setContentType("application/json");
    std::ostream& out = response.send();

    QJsonObject responseData {{
        "error", "404"
    }};

    out << JsonSettings::serializeJson(responseData).toStdString();
    out.flush();
}
