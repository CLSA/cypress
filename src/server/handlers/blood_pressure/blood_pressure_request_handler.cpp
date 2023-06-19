#include "blood_pressure_request_handler.h"
#include "cypress_application.h"

#include "auxiliary/json_settings.h"

void BloodPressureRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
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

    qDebug() << QJsonDocument(requestData).toJson(QJsonDocument::Indented);

    QJsonObject responseJson = getResponseData();
    QString responseData = JsonSettings::serializeJson(responseJson);

    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    response.setContentType("application/json");

    std::ostream& out = response.send();

    CypressApplication::getInstance().server -> requestTestStart(
        Constants::MeasureType::Blood_Pressure, requestData, responseJson["sessionId"].toString()
    );

    out << responseData.toStdString();
    out.flush();
}

bool BloodPressureRequestHandler::isValidInputData(const QJsonObject& inputData)
{
    return DefaultRequestHandler::isValidInputData(inputData);
}
