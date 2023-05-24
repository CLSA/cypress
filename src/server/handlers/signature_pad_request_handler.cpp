#include "signature_pad_request_handler.h"

#include <sstream>

void SignaturePadRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    try {
        QJsonObject requestData = getRequestData(request);

        // prepare response
        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        const Constants::MeasureType measureType = Constants::MeasureType::Signature;
        const QJsonObject responseJSON = getResponseData();
        const QString sessionId = responseJSON.value("sessionId").toString();
        const QString responseData = JsonSettings::serializeJson(responseJSON);

        // start test
        CypressApplication::getInstance().server -> requestTestStart(measureType, requestData);

        // send response with uuid body
        std::ostream& out = response.send();
        out << "\"" << sessionId.toStdString() << "\"";
        out.flush();
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }
}



