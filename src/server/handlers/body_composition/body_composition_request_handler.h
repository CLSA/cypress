#ifndef BODYCOMPOSITIONREQUESTHANDLER_H
#define BODYCOMPOSITIONREQUESTHANDLER_H

#include "server/default_request_handler.h"

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

class BodyCompositionRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // BODYCOMPOSITIONREQUESTHANDLER_H
