#ifndef FRAX_REQUEST_HANDLER_H
#define FRAX_REQUEST_HANDLER_H

#include "server/DefaultRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"


class FraxRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // FRAX_REQUEST_HANDLER_H
