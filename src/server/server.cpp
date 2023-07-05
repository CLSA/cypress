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

    server.reset(new HTTPServer(pFactory, portNumber, pParams));
    moveToThread(&serverThread);
}

Server::~Server()
{
    stop();
}

void Server::start()
{
    serverThread.start();
    server->start();
    qInfo() << "listening at " + QString::fromStdString(server->socket().address().toString());
}

void Server::stop()
{
    server->stop();
    serverThread.quit();
    serverThread.wait();
}
