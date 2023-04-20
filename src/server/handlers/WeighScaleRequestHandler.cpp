#include "CypressApplication.h"
#include "WeighScaleRequestHandler.h"
#include "auxiliary/JsonSettings.h"


void WeighScaleRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    Q_UNUSED(request)
    try {
        QString responseData = JsonSettings::serializeJson(getResponseData());
        CypressApplication::getInstance().server -> requestTestStart(Constants::MeasureType::Grip_Strength, QJsonObject {});

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
