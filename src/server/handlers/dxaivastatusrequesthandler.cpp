#include "dxaivastatusrequesthandler.h"

void DXAIvaStatusRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
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
