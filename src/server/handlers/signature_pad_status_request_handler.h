#ifndef SIGNATURE_PAD_STATUS_REQUEST_HANDLER_H
#define SIGNATURE_PAD_STATUS_REQUEST_HANDLER_H

#include "server/DefaultRequestHandler.h"

class SignaturePadStatusRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // SIGNATURE_PAD_STATUS_REQUEST_HANDLER_H
