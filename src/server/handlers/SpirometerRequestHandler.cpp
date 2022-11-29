#include "SpirometerRequestHandler.h"
#include "CypressApplication.h"

void SpirometerRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    try {
        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        std::ostream& out = response.send();

        CypressApplication::restApiServer -> requestTestStart(Constants::MeasureType::typeSpirometer);

        out.flush();
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }

}
