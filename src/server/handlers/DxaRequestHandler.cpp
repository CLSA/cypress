#include "DxaRequestHandler.h"
#include "CypressApplication.h"
#include <QDebug>

void DxaRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    Q_UNUSED(request)
    try {
        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        std::ostream& out = response.send();

        CypressApplication::restApiServer -> requestTestStart(Constants::MeasureType::typeDxaWholeBody, QJsonObject {});

        out.flush();
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }
}
