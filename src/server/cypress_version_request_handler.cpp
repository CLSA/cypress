#include "cypress_version_request_handler.h"

#include "auxiliary/json_settings.h"

#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

void CypressVersionRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    std::string uri = request.getURI();

    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    response.setContentType("application/json");
    std::ostream& out = response.send();

    QString cypressVersion = CypressSettings::getVersion();

    QJsonObject responseData {{
        "version", cypressVersion
    }};

    out << JsonSettings::serializeJson(responseData).toStdString();
    out.flush();
}
