#include <string>
#include <iostream>
#include <QThread>

#include "Poco/Net/HTTPServer.h"
#include "InstrumentRequestHandlerFactory.h"
#include "server.h"


using namespace Poco::Net;

Server::Server()
{
    Poco::Net::HTTPRequestHandlerFactory::Ptr pFactory = new InstrumentRequestHandlerFactory;
    Poco::UInt16 portNumber = 8000;
    Poco::Net::HTTPServerParams::Ptr pParams = new Poco::Net::HTTPServerParams;

    server = new HTTPServer(pFactory, portNumber, pParams);
    moveToThread(&serverThread);
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
