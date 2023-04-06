#include "SpirometerRequestHandler.h"
#include "CypressApplication.h"
#include "auxiliary/JsonSettings.h"

void SpirometerRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    Q_UNUSED(request)
    try {
        QString responseData = JsonSettings::serializeJson(getResponseData());
        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        CypressApplication::restApiServer -> requestTestStart(Constants::MeasureType::typeSpirometer, QJsonObject {});
        std::ostream& out = response.send();


        out.flush();
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }

}
