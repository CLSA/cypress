#ifndef OCT_LEFT_REQUEST_HANDLER_H
#define OCT_LEFT_REQUEST_HANDLER_H

#include "server/default_request_handler.h"

class OCTLeftRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // OCT_LEFT_REQUEST_HANDLER_H
