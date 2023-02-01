#include <QJsonObject>

#include "BodyCompositionRequestHandler.h"
#include "CypressApplication.h"

#include "auxiliary/JsonSettings.h"

void BodyCompositionRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    try {
        QString responseData = JsonSettings::serializeJson(getResponseData());

        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        std::ostream& out = response.send();

        CypressApplication::restApiServer -> requestTestStart(
            Constants::MeasureType::typeBody_Composition
        );

        out << responseData.toStdString();

        out.flush();
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }
}
