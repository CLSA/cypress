#include "DxaForearmBoneDensityHandler.h"
#include "CypressApplication.h"
#include <QDebug>

#include "auxiliary/JsonSettings.h"

void DxaForearmBoneDensityHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    Q_UNUSED(request)
    try {
        QString responseData = JsonSettings::serializeJson(getResponseData());

        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        CypressApplication::restApiServer -> requestTestStart(Constants::MeasureType::typeDxaForearmBone, QJsonObject {});

        std::ostream& out = response.send();
        out << responseData.toStdString();
        out.flush();
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }
}
