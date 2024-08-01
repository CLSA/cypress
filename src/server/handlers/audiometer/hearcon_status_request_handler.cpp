#include "hearcon_status_request_handler.h"

#include "managers/hearcon_manager.h"
#include "auxiliary/json_settings.h"

void HearconStatusRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    Q_UNUSED(request)
    try {
        QString responseData = JsonSettings::serializeJson(QJsonObject {{
            "status", !HearconManager::config.hasErrors() ? "available" : "unavailable"
        }});

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
