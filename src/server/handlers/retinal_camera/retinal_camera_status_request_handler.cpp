#include "retinal_camera_status_request_handler.h"
#include "managers/retinal_camera/retinal_camera_manager.h"
#include "auxiliary/json_settings.h"

void RetinalCameraStatusRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    Q_UNUSED(request)
    try {
        QJsonObject responseData {
            {"status", RetinalCameraManager::isInstalled() ? "available" : "unavailable" }
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
