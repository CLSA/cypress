#include "CypressApplication.h"
#include "auxiliary/JsonSettings.h"
#include "ultrasound_request_handler.h"

void UltrasoundRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    try {
        QJsonObject requestData = getRequestData(request);
        QString responseData = JsonSettings::serializeJson(getResponseData());

        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");
        CypressApplication::getInstance().server -> requestTestStart(Constants::MeasureType::CarotidIntima, requestData);

        std::ostream& out = response.send();
        out << responseData.toStdString();
        out.flush();
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }

}
