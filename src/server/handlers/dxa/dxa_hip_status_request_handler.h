#ifndef DXAHIPSTATUSREQUESTHANDLER_H
#define DXAHIPSTATUSREQUESTHANDLER_H

#include "../../default_request_handler.h"

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

class DxaHipStatusRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // DXAHIPSTATUSREQUESTHANDLER_H
