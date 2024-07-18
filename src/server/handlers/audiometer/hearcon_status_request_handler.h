#ifndef HEARCON_STATUS_REQUEST_HANDLER_H
#define HEARCON_STATUS_REQUEST_HANDLER_H

#include "server/default_request_handler.h"

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

class HearconStatusRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // HEARCON_STATUS_REQUEST_HANDLER_H
