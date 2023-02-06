#include "cdttstatusrequesthandler.h"

#include "auxiliary/JsonSettings.h"

void CDTTStatusRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    try {
        QJsonObject responseJson = {{"available", true}};
        QString responseData = JsonSettings::serializeJson(responseJson);

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