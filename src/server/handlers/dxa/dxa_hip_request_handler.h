#ifndef DXAHIPREQUESTHANDLER_H
#define DXAHIPREQUESTHANDLER_H

#include "../../default_request_handler.h"

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

class DxaHipRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // DXAHIPREQUESTHANDLER_H
