#include "cdtt_status_request_handler.h"
#include "managers/cdtt/cdtt_manager.h"

#include "auxiliary/json_settings.h"

void CDTTStatusRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    Q_UNUSED(request)
    try {
        QJsonObject responseJson {{"status", CDTTManager::isInstalled() ? "available" : "unavailable" }};
        QString responseData = JsonSettings::serializeJson(responseJson);

        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        std::ostream& out = response.send();

        out << responseData.toStdString();

        out.flush();
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }
}
