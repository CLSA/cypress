#include "audiometerrequesthandler.h"
#include "CypressApplication.h"

void AudiometerRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    try {
        QJsonObject responseData;

        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        std::ostream& out = response.send();

        CypressApplication::restApiServer -> requestTestStart(
            Constants::MeasureType::typeAudiometer
        );

        out.flush();
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }
}
