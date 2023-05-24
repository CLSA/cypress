#include "signature_pad_status_request_handler.h"

#include <QString>
#include <QJsonObject>

#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

#include "auxiliary/JsonSettings.h"

void SignaturePadStatusRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    Q_UNUSED(request)
    try {
        QJsonObject responseData {
            {"status", "Available"}
        };

        QString jsonResponse = JsonSettings::serializeJson(responseData);

        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        std::ostream& out = response.send();

        out << jsonResponse.toStdString();
        out.flush();
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }
};
