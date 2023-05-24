#include "spirometer_request_handler.h"
#include "CypressApplication.h"
#include "auxiliary/JsonSettings.h"

void SpirometerRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    try {
        QJsonObject requestData = getRequestData(request);
        QString responseData = JsonSettings::serializeJson(getResponseData());

        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        CypressApplication::getInstance().server -> requestTestStart(Constants::MeasureType::Spirometer, requestData);
        std::ostream& out = response.send();

        out.flush();
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }

}
