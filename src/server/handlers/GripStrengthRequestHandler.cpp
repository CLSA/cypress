#include <iostream>

#include <QString>
#include <QDebug>
#include <QObject>
#include <QApplication>

#include "GripStrengthRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPResponse.h"
#include "CypressApplication.h"


void GripStrengthRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    try {
        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        std::ostream& out = response.send();

        CypressApplication::restApiServer -> requestTestStart(Constants::MeasureType::typeGrip_Strength);

        out.flush();
    }
    catch (std::exception& e)
    {
        qDebug() << e.what();
    }
}