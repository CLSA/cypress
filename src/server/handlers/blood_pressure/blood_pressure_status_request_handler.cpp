#include "blood_pressure_status_request_handler.h"
#include "managers/blood_pressure/blood_pressure_manager.h"

#include "auxiliary/json_settings.h"

void BloodPressureStatusRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    Q_UNUSED(request)
    try {
        QJsonObject responseData {
            {"status", BloodPressureManager::isInstalled() ? "available" : "unavailable"}
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
