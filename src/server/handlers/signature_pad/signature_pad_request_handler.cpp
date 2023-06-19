#include <sstream>

#include "signature_pad_request_handler.h"
#include "auxiliary/Constants.h"
#include "auxiliary/json_settings.h"
#include "cypress_application.h"

void SignaturePadRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
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

    // prepare response
    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    response.setContentType("application/json");

    const Constants::MeasureType measureType = Constants::MeasureType::Signature;
    const QJsonObject responseJSON = getResponseData();
    const QString sessionId = responseJSON.value("sessionId").toString();
    const QString responseData = JsonSettings::serializeJson(responseJSON);

    // start test
    CypressApplication::getInstance().server -> requestTestStart(measureType, requestData, sessionId);

    // send response with uuid body
    std::ostream& out = response.send();
    out << "\"" << sessionId.toStdString() << "\"";
    out.flush();
}

bool SignaturePadRequestHandler::isValidInputData(const QJsonObject& inputData)
{
    return DefaultRequestHandler::isValidInputData(inputData);
}
