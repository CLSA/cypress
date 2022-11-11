#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QObject>
#include "Poco/Net/HTTPServer.h"

class HTTPServer : public Poco::Net::HTTPServer, QObject
{
    Q_OBJECT
public:
    HTTPServer(Poco::Net::HTTPRequestHandlerFactory::Ptr pFactory, const Poco::Net::ServerSocket & socket, Poco::Net::HTTPServerParams::Ptr pParams, const Server& server);
};

#endif // HTTPSERVER_H
