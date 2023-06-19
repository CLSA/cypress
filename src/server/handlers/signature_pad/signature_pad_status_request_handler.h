#ifndef SIGNATURE_PAD_STATUS_REQUEST_HANDLER_H
#define SIGNATURE_PAD_STATUS_REQUEST_HANDLER_H

#include "server/default_request_handler.h"

class SignaturePadStatusRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // SIGNATURE_PAD_STATUS_REQUEST_HANDLER_H
