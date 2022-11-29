#ifndef CDTTREQUESTHANDLER_H
#define CDTTREQUESTHANDLER_H

#include "server/DefaultRequestHandler.h"

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"


class CDTTRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // CDTTREQUESTHANDLER_H
