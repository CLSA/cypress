#ifndef AUDIOMETERREQUESTHANDLER_H
#define AUDIOMETERREQUESTHANDLER_H

#include "server/DefaultRequestHandler.h"

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

class AudiometerRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // AUDIOMETERREQUESTHANDLER_H
