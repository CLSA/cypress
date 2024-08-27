#include "server/handlers/general_proxy_consent/general_proxy_consent_status_request_handler.h"
#include "managers/general_proxy_form/general_proxy_manager.h"
#include "auxiliary/json_settings.h"

void GeneralProxyConsentStatusRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    Q_UNUSED(request)
    try {
        QJsonObject responseData {{
            "status", !GeneralProxyManager::config.hasErrors() ? "available" : "unavailable"
        }};

        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        std::ostream& out = response.send();
        out << JsonSettings::serializeJson(responseData).toStdString();
        out.flush();
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }
}
