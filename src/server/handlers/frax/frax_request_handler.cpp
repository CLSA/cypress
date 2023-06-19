#include <QDebug>

#include "cypress_application.h"
#include "frax_request_handler.h"

#include "auxiliary/json_settings.h"

void FraxRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
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

    CypressApplication::getInstance().server -> requestTestStart(Constants::MeasureType::Frax, requestData, responseJson["sessionId"].toString());

    std::ostream& out = response.send();
    out << responseData.toStdString();
    out.flush();
}

bool FraxRequestHandler::isValidInputData(const QJsonObject& inputData)
{
    return DefaultRequestHandler::isValidInputData(inputData);
}
