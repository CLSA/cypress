#include "dxa_session_request_handler.h"
#include "cypress_application.h"
#include "server/sessions/dxa/dxa_session.h"

void DxaSessionRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request,
                                             Poco::Net::HTTPServerResponse &response)
{
    Q_UNUSED(request)
    try {
        QString responseData = "";
        QList<QSharedPointer<CypressSession>> activeSessions = Cypress::getInstance()
                                                                   .getActiveSessions();

        for (const QSharedPointer<CypressSession> &session : activeSessions) {
            auto dxaSession = qSharedPointerCast<DXASession>(session);
            if (dxaSession) {
                responseData = dxaSession->getWebpageContents();
            }
        }

        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("text/html");

        std::ostream &out = response.send();
        out << responseData.toStdString();
        out.flush();
    } catch (...) {
    }
}
