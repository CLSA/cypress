#ifndef VIVID_IQ_STATUS_REQUEST_HANDLER_H
#define VIVID_IQ_STATUS_REQUEST_HANDLER_H

#include "server/default_request_handler.h"

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

class VividIQStatusRequestHandler : public DefaultRequestHandler
{
    // HTTPRequestHandler interface
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response);
};

#endif // VIVID_IQ_STATUS_REQUEST_HANDLER_H
