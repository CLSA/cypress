#ifndef GRIP_STRENGTH_REQUEST_HANDLER_H
#define GRIP_STRENGTH_REQUEST_HANDLER_H

#include "server/DefaultRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

class GripStrengthRequestHandler: public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // GRIP_STRENGTH_REQUEST_HANDLER_H
