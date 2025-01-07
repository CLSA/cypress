#include "cypress_status_request_handler.h"
#include "auxiliary/json_settings.h"
#include "cypress_application.h"

#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

#include <QJsonDocument>
#include <QJsonObject>


void CypressStatusRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    std::string uri = request.getURI();

    response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
    response.setContentType("application/json");
    std::ostream& out = response.send();

    QJsonObject responseData = Cypress::getInstance().getStatus();

    out << JsonSettings::serializeJson(responseData).toStdString();
    out.flush();
}
