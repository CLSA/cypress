#include "weigh_scale_status_request_handler.h"
#include "managers/weigh_scale/weigh_scale_manager.h"
#include "auxiliary/json_settings.h"

void WeighScaleStatusRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    Q_UNUSED(request)
    try {
        QJsonObject responseData {{
            "status", WeighScaleManager::isInstalled() ? "available" : "unavailable"
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
