#include "RetinalCameraRequestHandler.h"
#include "CypressApplication.h"

#include <QDebug>

void RetinalCameraRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    qDebug() << "RetinalCameraRequestHandler";
    try {
        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        std::ostream& out = response.send();

        CypressApplication::restApiServer -> requestTestStart(Constants::MeasureType::typeRetinal_Camera);

        out.flush();
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }
}
