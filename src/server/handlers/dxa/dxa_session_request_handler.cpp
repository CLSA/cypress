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
            DXASession *dxaSession = static_cast<DXASession *>(session.get());
            if (dxaSession) {
                responseData = dxaSession->getWebpageContents();
            }
        }

        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        std::ostream &out = response.send();
        out << responseData.toStdString();
        out.flush();
    } catch (...) {
    }
}
