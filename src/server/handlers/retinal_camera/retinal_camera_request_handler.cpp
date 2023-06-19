#include <QDebug>

#include "cypress_application.h"
#include "auxiliary/json_settings.h"
#include "retinal_camera_request_handler.h"

void RetinalCameraRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
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

    // prepare response
    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    response.setContentType("application/json");

    const Constants::MeasureType measureType = Constants::MeasureType::Retinal_Camera;
    const QJsonObject responseJSON = getResponseData();
    const QString sessionId = responseJSON.value("id").toString();

    // start test
    CypressApplication::getInstance().server -> requestTestStart(measureType, requestData, responseJson["sessionId"].toString());

    // send response with uuid body
    std::ostream& out = response.send();
    out << responseData.toStdString();
    out.flush();
}

bool RetinalCameraRequestHandler::isValidInputData(const QJsonObject& inputData)
{
    return DefaultRequestHandler::isValidInputData(inputData);
}
