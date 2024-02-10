#include "cypress_application.h"

#include "cdtt_request_handler.h"
#include "auxiliary/json_settings.h"


void CDTTRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    try {
        QJsonObject requestData = getRequestData(request);
        QString sessionId = Cypress::getInstance().httpServer->requestDevice(Constants::MeasureType::CDTT, requestData);

        QJsonObject data = getResponseData(sessionId);
        QString responseData = JsonSettings::serializeJson(data);

        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        std::ostream& out = response.send();
        out << responseData.toStdString();
        out.flush();
    } catch (const NotInstalledError &exception) {
        response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
        response.setContentType("application/json");

        std::ostream &out = response.send();
        out << exception.what();
        out.flush();
    } catch (const NotAvailableError &exception) {
        response.setStatus(Poco::Net::HTTPResponse::HTTP_CONFLICT);
        response.setContentType("application/json");

        std::ostream &out = response.send();
        out << exception.what();
        out.flush();
    } catch (const ValidationError &exception) {
        response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        response.setContentType("application/json");

        std::ostream &out = response.send();
        out << exception.what();
        out.flush();
    }
}
