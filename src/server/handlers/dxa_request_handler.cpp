#include "dxa_request_handler.h"
#include "CypressApplication.h"

#include <QDebug>

#include "auxiliary/JsonSettings.h"


void DxaRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    try {
        QJsonObject requestData = getRequestData(request);
        QString responseData = JsonSettings::serializeJson(getResponseData());

        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        CypressApplication::getInstance().server -> requestTestStart(Constants::MeasureType::DxaWholeBody, requestData);

        std::ostream& out = response.send();
        out << responseData.toStdString();
        out.flush();
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }
}
