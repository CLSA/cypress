#include <QThread>
#include <QSettings>
#include <QDebug>

#include <string>
#include <iostream>

#include "Poco/Net/HTTPServer.h"
#include "server/handlers/InstrumentRequestHandlerFactory.h"
#include "server/Server.h"


using namespace Poco::Net;



Server::Server()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");

    HTTPRequestHandlerFactory::Ptr pFactory = new InstrumentRequestHandlerFactory;
    HTTPServerParams::Ptr pParams = new HTTPServerParams;
    Poco::UInt16 portNumber = settings.value("server/port", 8000).toInt();


    server = new HTTPServer(pFactory, portNumber, pParams);
    moveToThread(&serverThread);
}

void Server::requestTestStart(Constants::MeasureType type)
{
    emit startTest(type);
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
