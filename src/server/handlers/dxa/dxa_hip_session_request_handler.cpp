#include "dxa_hip_session_request_handler.h"

#include "cypress_application.h"
#include "server/sessions/dxa/dxa_hip_session.h"

void DxaHipSessionRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request,
                                                Poco::Net::HTTPServerResponse &response)
{
    Q_UNUSED(request)
    try {
        QList<QSharedPointer<CypressSession>> activeSessions = Cypress::getInstance()
                                                                   .getActiveSessions();

        QString responseData = "";
        for (const QSharedPointer<CypressSession> &session : activeSessions) {
            auto dxaHipSession = qSharedPointerCast<DxaHipSession>(session);
            if (dxaHipSession) {
                responseData = dxaHipSession->getWebpageContents();
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
