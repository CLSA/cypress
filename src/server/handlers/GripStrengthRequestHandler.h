#ifndef GRIPSTRENGTHREQUESTHANDLER_H
#define GRIPSTRENGTHREQUESTHANDLER_H

#include "server/DefaultRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

class GripStrengthRequestHandler: public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // GRIPSTRENGTHREQUESTHANDLER_H
