#ifndef RETINALCAMERARIGHTREQUESTHANDLER_H
#define RETINALCAMERARIGHTREQUESTHANDLER_H

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

#include "server/default_request_handler.h"

class RetinalCameraRightRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;
};

#endif // RETINALCAMERARIGHTREQUESTHANDLER_H
