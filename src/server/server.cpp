#include <QThread>
#include <QSettings>
#include <QDebug>

#include "auxiliary/json_settings.h"

#include <string>
#include <iostream>

#include "Poco/Net/HTTPServer.h"
#include "server/instrument_request_handler_factory.h"
#include "server/Server.h"

#include "cypress_application.h"

using namespace Poco::Net;


Server::Server()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");

    HTTPRequestHandlerFactory::Ptr pFactory = new InstrumentRequestHandlerFactory;
    HTTPServerParams::Ptr pParams = new HTTPServerParams;
    Poco::UInt16 portNumber = settings.value("server/port", 9000).toInt();

    server = new HTTPServer(pFactory, portNumber, pParams);
    moveToThread(&serverThread);
}

void Server::requestTestStart(const Constants::MeasureType& type, const QJsonObject& requestData, const QString& sessionId)
{
    emit startTest(type, requestData, sessionId);
}


bool Server::sendResults(const QJsonObject &results)
{
    QString jsonString = JsonSettings::serializeJson(results);
    return true;
}


Server::~Server()
{
    serverThread.quit();
    serverThread.wait();
    delete server;
}

void Server::start()
{
    serverThread.start();
    server->start();
}

void Server::stop()
{
    server->stop();
    serverThread.quit();
    serverThread.wait();
}
