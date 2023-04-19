

#include <QString>
#include <QDebug>

#include "DxaDualHipBoneDensityHandler.h"
#include "CypressApplication.h"
#include "auxiliary/JsonSettings.h"

void DxaDualHipBoneDensityHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    try {
        QString responseData = JsonSettings::serializeJson(getResponseData());
        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        CypressApplication::getInstance().server -> requestTestStart(Constants::MeasureType::DxaDualHip, QJsonObject {});

        std::ostream& out = response.send();
        out << responseData.toStdString();
        out.flush();
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }
}
