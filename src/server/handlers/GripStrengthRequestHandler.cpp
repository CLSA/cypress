#include <iostream>

#include <QString>
#include <QDebug>
#include <QObject>
#include <QApplication>

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPResponse.h"

#include "CypressApplication.h"
#include "auxiliary/JsonSettings.h"
#include "GripStrengthRequestHandler.h"


void GripStrengthRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    try {
        QString responseData = JsonSettings::serializeJson(getResponseData());

        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        CypressApplication::restApiServer -> requestTestStart(Constants::MeasureType::typeGrip_Strength, "");

        std::ostream& out = response.send();
        out << responseData.toStdString();
        out.flush();
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }
}
