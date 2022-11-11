#include "server.h"

#include "InstrumentRequestHandlerFactory.h"

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/ServerSocket.h"
#include <string>
#include <iostream>

using namespace Poco::Net;

Server::Server()
{
}

Server::~Server()
{
}

void Server::initialize(Application& self)
{
        loadConfiguration();
        ServerApplication::initialize(self);
}

void Server::uninitialize()
{
        ServerApplication::uninitialize();
}

int Server::main(const std::vector<std::string>& args)
{
        HTTPServer serv(new InstrumentRequestHandlerFactory, ServerSocket(Server::DEFAULT_PORT), new HTTPServerParams);

        serv.start();
        waitForTerminationRequest();
        serv.stop();

        return Application::EXIT_OK;
}
