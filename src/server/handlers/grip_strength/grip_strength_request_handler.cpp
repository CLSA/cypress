#include <iostream>

#include <QString>
#include <QDebug>
#include <QObject>
#include <QApplication>

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPResponse.h"

#include "cypress_application.h"
#include "auxiliary/json_settings.h"
#include "grip_strength_request_handler.h"


void GripStrengthRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    try {
        QJsonObject requestData = getRequestData(request);
        QString sessionId = Cypress::getInstance().httpServer->requestDevice(Constants::MeasureType::Grip_Strength, requestData);

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
