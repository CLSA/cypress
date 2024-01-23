#ifndef DXASESSIONREQUESTHANDLER_H
#define DXASESSIONREQUESTHANDLER_H

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "server/default_request_handler.h"

class DxaSessionRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request,
                       Poco::Net::HTTPServerResponse &response) override;
};

#endif // DXASESSIONREQUESTHANDLER_H
