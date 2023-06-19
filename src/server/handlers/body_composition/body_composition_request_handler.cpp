#include <QJsonObject>

#include "body_composition_request_handler.h"
#include "cypress_application.h"

#include "auxiliary/json_settings.h"

void BodyCompositionRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
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

    qDebug() << QJsonDocument(requestData).toJson(QJsonDocument::Indented);

    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    response.setContentType("application/json");

    std::ostream& out = response.send();
    CypressApplication::getInstance().server -> requestTestStart(
        Constants::MeasureType::Body_Composition, requestData, responseJson["sessionId"].toString()
    );

    out << responseData.toStdString();
    out.flush();
}

bool BodyCompositionRequestHandler::isValidInputData(const QJsonObject& inputData)
{
    return DefaultRequestHandler::isValidInputData(inputData);
}
