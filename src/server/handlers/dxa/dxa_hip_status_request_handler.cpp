#include "dxa_hip_status_request_handler.h"
#include "managers/dxa/dxa_hip_manager.h"

#include "auxiliary/json_settings.h"


void DxaHipStatusRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    Q_UNUSED(request)
    try {
        QJsonObject responseData {
            {"status", !DxaHipManager::config.hasErrors() ? "available" : "unavailable"}
        };

        QString jsonResponse = JsonSettings::serializeJson(responseData);

        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        std::ostream& out = response.send();

        out << jsonResponse.toStdString();
        out.flush();
    }

    catch (std::exception& e)
    {
        qDebug() << e.what();
    }
}
