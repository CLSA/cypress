#ifndef GENERALPROXYCONSENTREQUESTHANDLER_H
#define GENERALPROXYCONSENTREQUESTHANDLER_H

#include "server/default_request_handler.h"

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

class GeneralProxyConsentRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // GENERALPROXYCONSENTREQUESTHANDLER_H
