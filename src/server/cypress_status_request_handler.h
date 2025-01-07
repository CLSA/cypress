#ifndef CYPRESS_STATUS_REQUEST_HANDLER_H
#define CYPRESS_STATUS_REQUEST_HANDLER_H

#include "default_request_handler.h"

class CypressStatusRequestHandler : public DefaultRequestHandler
{
    // HTTPRequestHandler interface
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response);
};

#endif // CYPRESS_STATUS_REQUEST_HANDLER_H
