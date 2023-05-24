#include <QJsonObject>

#include "body_composition_request_handler.h"
#include "CypressApplication.h"

#include "auxiliary/JsonSettings.h"

void BodyCompositionRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    try {
        QString responseData = JsonSettings::serializeJson(getResponseData());
        QJsonObject requestData = getRequestData(request);
        qDebug() << QJsonDocument(requestData).toJson(QJsonDocument::Indented);

        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        std::ostream& out = response.send();
        CypressApplication::getInstance().server -> requestTestStart(
                    Constants::MeasureType::Body_Composition, requestData
        );

        out << responseData.toStdString();
        out.flush();
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }
}
