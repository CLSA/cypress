#ifndef CYPRESS_VERSION_REQUEST_HANDLER_H
#define CYPRESS_VERSION_REQUEST_HANDLER_H

#include "default_request_handler.h"

class CypressVersionRequestHandler : public DefaultRequestHandler
{
    // HTTPRequestHandler interface
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response);
};

#endif // CYPRESS_VERSION_REQUEST_HANDLER_H
