#include "audiometer_request_handler.h"
#include "cypress_session.h"
#include "cypress_application.h"

#include "auxiliary/json_settings.h"

void AudiometerRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    QJsonObject requestData = getRequestData(request);

    try {
        CypressSession session(Constants::MeasureType::Audiometer, requestData);
    }
    catch (const InvalidInputDataException& exception)
    {
        qDebug() << exception.what();
    }

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

    QJsonObject data = getResponseData();
    QString responseData = JsonSettings::serializeJson(data);

    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    response.setContentType("application/json");
    
    //Cypress::getInstance().server -> requestTestStart(
    //    Constants::MeasureType::Audiometer, requestData, data["sessionId"].toString()
    //);

    std::ostream& out = response.send();
    out << responseData.toStdString();
    out.flush();
}

bool AudiometerRequestHandler::isValidInputData(const QJsonObject& inputData)
{
    return DefaultRequestHandler::isValidInputData(inputData);
}
