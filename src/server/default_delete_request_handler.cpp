#include "default_delete_request_handler.h"

#include "cypress_application.h"
#include <string>
#include <iostream>
#include <fstream>
#include <regex>

#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QDebug>

#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

void DefaultDeleteSessionRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    std::string uri = request.getURI();
    std::regex uuid_regex("[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}");
    std::smatch match;

    QString uuid;

    if(std::regex_search(uri, match, uuid_regex)) {
        std::cout << "UUID: " << match[0] << std::endl;
        uuid = QString::fromStdString(match[0]);
    }
    else {
        response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
        response.setContentType("application/json");

        std::ostream& out = response.send();
        out.flush();
        return;
    }

    Cypress::getInstance().httpServer->endSession(uuid);

    response.setStatus(Poco::Net::HTTPResponse::HTTP_ACCEPTED);
    response.setContentType("application/json");

    std::ostream& out = response.send();
    out.flush();
}
