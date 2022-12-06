#include "DxaLateralBoneDensityHandler.h"

#include "CypressApplication.h"
#include <QDebug>

void DxaLateralBoneDensityHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    try {
        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        std::ostream& out = response.send();

        CypressApplication::restApiServer -> requestTestStart(Constants::MeasureType::typeDxaLateralBone);

        out.flush();
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }
}