#ifndef TONOMETERREQUESTHANDLER_H
#define TONOMETERREQUESTHANDLER_H

#include <Poco/Net/HTTPRequestHandler.h>
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

class TonometerRequestHandler : public Poco::Net::HTTPRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // TONOMETERREQUESTHANDLER_H
