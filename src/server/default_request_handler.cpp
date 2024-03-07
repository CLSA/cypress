#include "default_request_handler.h"
#include "auxiliary/json_settings.h"

#include <Poco/StreamCopier.h>
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QDebug>

#include <string>
#include <iostream>

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
    std::istream& iStream = request.stream();
    std::string requestData;
    Poco::StreamCopier::copyToString(iStream, requestData);

    QString qRequestData = QString::fromStdString(requestData);

    qDebug() << "URI" << request.getURI().c_str();
    qDebug() << "Origin" << request.get("Origin", "N/A").c_str();
    qDebug() << "Host" << request.get("Host", "N/A").c_str();
    qDebug() << "Content-Type" << request.get("Content-Type", "N/A").c_str();

    QJsonDocument doc = QJsonDocument::fromJson(qRequestData.toUtf8());
    QJsonObject json = doc.object();

    return json;
}

QJsonObject DefaultRequestHandler::getResponseData(const QString& sessionId) {
    QJsonObject responseData {
        { "sessionId", sessionId }
    };

    return responseData;
}
