#ifndef DXAHIPSESSIONREQUESTHANDLER_H
#define DXAHIPSESSIONREQUESTHANDLER_H

#include "server/default_request_handler.h"

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

class DxaHipSessionRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request,
                       Poco::Net::HTTPServerResponse &response) override;
};

#endif // DXAHIPSESSIONREQUESTHANDLER_H
