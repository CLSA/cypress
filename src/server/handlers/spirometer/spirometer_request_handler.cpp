#include "spirometer_request_handler.h"
#include "cypress_application.h"
#include "auxiliary/json_settings.h"

void SpirometerRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    QJsonObject requestData = getRequestData(request);
    if (!isValidInputData(requestData))
    {
        response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        response.setContentType("application/json");

        std::ostream& out = response.send();
        out.flush();

        return;
    }

    if (isBusy())
    {
        response.setStatus(Poco::Net::HTTPResponse::HTTP_CONFLICT);
        response.setContentType("application/json");

        std::ostream& out = response.send();
        out.flush();

        return;
    }
    QJsonObject responseJson = getResponseData();
    QString responseData = JsonSettings::serializeJson(responseJson);

    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    response.setContentType("application/json");

    CypressApplication::getInstance().server -> requestTestStart(Constants::MeasureType::Spirometer, requestData, responseJson["sessionId"].toString());
    std::ostream& out = response.send();

    out.flush();
}

bool SpirometerRequestHandler::isValidInputData(const QJsonObject& inputData)
{
    return DefaultRequestHandler::isValidInputData(inputData);
}
