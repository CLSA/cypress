#ifndef TONOMETER_REQUEST_HANDLER_H
#define TONOMETER_REQUEST_HANDLER_H

#include "server/default_request_handler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

class TonometerRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // TONOMETER_REQUEST_HANDLER_H
