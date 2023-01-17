#include "bloodpressurerequesthandler.h"
#include "CypressApplication.h"

void BloodPressureRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    try {
        QJsonObject responseData;

        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        std::ostream& out = response.send();

        CypressApplication::restApiServer -> requestTestStart(
            Constants::MeasureType::typeBlood_Pressure
        );

        out.flush();
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }
}
