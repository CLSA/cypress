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
    Poco::Net::HTTPRequestHandlerFactory::Ptr pFactory = new InstrumentRequestHandlerFactory;
    Poco::UInt16 portNumber = 8000;
    Poco::Net::HTTPServerParams::Ptr pParams = new Poco::Net::HTTPServerParams;

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");

    server = new HTTPServer(pFactory, portNumber, pParams);
    moveToThread(&serverThread);
}

void Server::requestTestStart()
{
    qDebug() << "Requested test start";
    emit testStart();
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
