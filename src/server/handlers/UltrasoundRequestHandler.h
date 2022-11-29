#ifndef ULTRASOUNDREQUESTHANDLER_H
#define ULTRASOUNDREQUESTHANDLER_H

#include <Poco/Net/HTTPRequestHandler.h>
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

#include "server/DefaultRequestHandler.h"

class UltrasoundRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // ULTRASOUNDREQUESTHANDLER_H
