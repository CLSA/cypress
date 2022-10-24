#ifndef GRIPSTRENGTHREQUESTHANDLER_H
#define GRIPSTRENGTHREQUESTHANDLER_H

#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "server.h"

extern Server* server;
class GripStrengthRequestHandler: public Poco::Net::HTTPRequestHandler
{
public:
    GripStrengthRequestHandler();
    ~GripStrengthRequestHandler();

    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response);
};

#endif // GRIPSTRENGTHREQUESTHANDLER_H
