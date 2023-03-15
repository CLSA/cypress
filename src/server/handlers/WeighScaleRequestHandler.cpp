#include "CypressApplication.h"
#include "WeighScaleRequestHandler.h"
#include "auxiliary/JsonSettings.h"


void WeighScaleRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    try {
        QString responseData = JsonSettings::serializeJson(getResponseData());
        CypressApplication::restApiServer -> requestTestStart(Constants::MeasureType::typeGrip_Strength, QJsonObject {});

        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        std::ostream& out = response.send();
        out << responseData.toStdString();
        out.flush();
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }

}
