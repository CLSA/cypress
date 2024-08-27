#ifndef MAC5_STATUS_REQUEST_HANDLER_H
#define MAC5_STATUS_REQUEST_HANDLER_H

#include "server/default_request_handler.h"

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

class Mac5StatusRequestHandler : public DefaultRequestHandler
{
    // HTTPRequestHandler interface
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response);
};

#endif // MAC5_STATUS_REQUEST_HANDLER_H
