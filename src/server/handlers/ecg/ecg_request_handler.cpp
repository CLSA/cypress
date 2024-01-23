#include <QDebug>

#include "cypress_application.h"
#include "auxiliary/json_settings.h"
#include "ecg_request_handler.h"

void ECGRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    try {
        QJsonObject requestData = getRequestData(request);
        QString sessionId = Cypress::getInstance().httpServer->requestDevice(Constants::MeasureType::ECG, requestData);

        QJsonObject data = getResponseData(sessionId);
        QString responseData = JsonSettings::serializeJson(data);

        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        std::ostream& out = response.send();
        out << responseData.toStdString();
        out.flush();
    } catch (const NotInstalledError &exception) {
        response.setStatus(Poco::Net::HTTPResponse::HTTP_FAILED_DEPENDENCY);
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
