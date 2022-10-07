#include "server.h"

#include "InstrumentRequestHandlerFactory.h"

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/ServerSocket.h"
#include <string>
#include <iostream>

using namespace Poco::Net;

Server::Server()
{
   m_httpServer = new HTTPServer(new InstrumentRequestHandlerFactory, ServerSocket(Server::DEFAULT_PORT), new HTTPServerParams);
}

Server::~Server()
{
   delete m_httpServer;
}

std::string Server::getAddress()
{
    return "";
}

int Server::getPort()
{
    return 0;
}

void Server::start()
{
    m_httpServer->start();
    waitForTerminationRequest();
    end();
}

void Server::end()
{
    m_httpServer->stop();
}

void Server::getConfig()
{
}

void Server::setConfig()
{
}

void Server::addHandler()
{
}

void Server::removeHandler()
{
}

std::string Server::toString()
{
    return "Server";
}
