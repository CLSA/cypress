#include "body_composition_status_request_handler.h"

void BodyCompositionStatusRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    Q_UNUSED(request)
    try {
        QJsonObject responseData {{}};

        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        std::ostream& out = response.send();

        out.flush();
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }
}