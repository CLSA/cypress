#include "bloodpressurerequesthandler.h"
#include "CypressApplication.h"

#include "auxiliary/JsonSettings.h"

void BloodPressureRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    Q_UNUSED(request)
    try {
        QString responseData = JsonSettings::serializeJson(getResponseData());

        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        std::ostream& out = response.send();

        CypressApplication::getInstance().server -> requestTestStart(
            Constants::MeasureType::Blood_Pressure,
                    QJsonObject {}
        );

        out << responseData.toStdString();

        out.flush();
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }
}
