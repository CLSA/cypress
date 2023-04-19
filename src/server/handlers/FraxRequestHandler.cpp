#include <QDebug>

#include "CypressApplication.h"
#include "FraxRequestHandler.h"

#include "auxiliary/JsonSettings.h"

void FraxRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    try {
        QString responseData = JsonSettings::serializeJson(getResponseData());

        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");
        CypressApplication::getInstance().server -> requestTestStart(Constants::MeasureType::Frax, QJsonObject {});

        std::ostream& out = response.send();
        out << responseData.toStdString();
        out.flush();
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }

}
