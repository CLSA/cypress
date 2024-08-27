#ifndef OCT_RIGHT_REQUEST_HANDLER_H
#define OCT_RIGHT_REQUEST_HANDLER_H

#include "server/default_request_handler.h"

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

class OCTRightRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response);
};

#endif // OCT_RIGHT_REQUEST_HANDLER_H
