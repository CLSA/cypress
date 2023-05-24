#include "blood_pressure_request_handler.h"
#include "CypressApplication.h"

#include "auxiliary/JsonSettings.h"

void BloodPressureRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    try {
        QJsonObject requestData = getRequestData(request);
        qDebug() << QJsonDocument(requestData).toJson(QJsonDocument::Indented);

        QString responseData = JsonSettings::serializeJson(getResponseData());

        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        std::ostream& out = response.send();

        CypressApplication::getInstance().server -> requestTestStart(
            Constants::MeasureType::Blood_Pressure, requestData
        );

        out << responseData.toStdString();

        out.flush();
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }
}
