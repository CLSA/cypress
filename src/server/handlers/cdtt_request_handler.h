#ifndef CDTT_REQUEST_HANDLER_H
#define CDTT_REQUEST_HANDLER_H

#include "server/DefaultRequestHandler.h"

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"


class CDTTRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // CDTT_REQUEST_HANDLER_H
