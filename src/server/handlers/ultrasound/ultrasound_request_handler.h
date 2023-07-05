#ifndef ULTRASOUND_REQUEST_HANDLER_H
#define ULTRASOUND_REQUEST_HANDLER_H

#include <Poco/Net/HTTPRequestHandler.h>
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

#include "server/default_request_handler.h"

class UltrasoundRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // ULTRASOUND_REQUEST_HANDLER_H
