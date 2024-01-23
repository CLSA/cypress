#include "frax_status_request_handler.h"
#include "auxiliary/json_settings.h"
#include "managers/frax/frax_manager.h"

void FraxStatusRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    Q_UNUSED(request)
    try {
        QJsonObject responseData {{
            "status", FraxManager::isInstalled() ? "available" : "unavailable"
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
