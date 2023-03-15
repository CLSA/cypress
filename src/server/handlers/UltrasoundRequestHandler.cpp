#include "CypressApplication.h"
#include "auxiliary/JsonSettings.h"
#include "server/handlers/UltrasoundRequestHandler.h"

void UltrasoundRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    try {
        QString responseData = JsonSettings::serializeJson(getResponseData());

        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");
        CypressApplication::restApiServer -> requestTestStart(Constants::MeasureType::typeCarotidIntima, QJsonObject {});

        std::ostream& out = response.send();
        out << responseData.toStdString();
        out.flush();
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }

}
