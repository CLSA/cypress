#include "retinalcamerastatusrequesthandler.h"
#include "auxiliary/JsonSettings.h"

void RetinalCameraStatusRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    try {
        QJsonObject responseData {
            {"status", "Available"}
        };

        QString jsonResponse = JsonSettings::serializeJson(responseData);

        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        std::ostream& out = response.send();

        out << jsonResponse.toStdString();
        out.flush();
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }
}
