#include "CypressApplication.h"
#include "weigh_scale_request_handler.h"
#include "auxiliary/JsonSettings.h"


void WeighScaleRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    try {
        QJsonObject requestData = getRequestData(request);
        QString responseData = JsonSettings::serializeJson(getResponseData());
        CypressApplication::getInstance().server -> requestTestStart(Constants::MeasureType::Weigh_Scale, requestData);

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
