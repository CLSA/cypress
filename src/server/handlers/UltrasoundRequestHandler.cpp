#include "server/handlers/UltrasoundRequestHandler.h"
#include "CypressApplication.h"

void UltrasoundRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    try {
        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        std::ostream& out = response.send();

        CypressApplication::restApiServer -> requestTestStart(Constants::MeasureType::typeUltrasound);

        out.flush();
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }

}
