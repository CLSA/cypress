#include <string>
#include <iostream>
#include <fstream>

#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QDebug>

#include <Poco/StreamCopier.h>
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

#include "default_request_handler.h"
#include "auxiliary/json_settings.h"

#include "cypress_application.h"

void DefaultRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    std::string uri = request.getURI();

    response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
    response.setContentType("application/json");
    std::ostream& out = response.send();

    QJsonObject responseData {{
        "error", "Not found"
    }};

    out << JsonSettings::serializeJson(responseData).toStdString();
    out.flush();
}

QJsonObject DefaultRequestHandler::getRequestData(Poco::Net::HTTPServerRequest &request) {
    // Step 1: Extract data from request
    std::istream& iStream = request.stream();
    std::string requestData;
    Poco::StreamCopier::copyToString(iStream, requestData);

    // Step 2: Convert to QString
    QString qRequestData = QString::fromStdString(requestData);

    // Step 3: Parse JSON data
    QJsonDocument doc = QJsonDocument::fromJson(qRequestData.toUtf8());

    // Step 4: Get JSON object
    QJsonObject json = doc.object();

    return json;
}

QJsonObject DefaultRequestHandler::getResponseData(const QString& sessionId) {
    QJsonObject responseData {
        { "sessionId", sessionId }
    };

    return responseData;
}
