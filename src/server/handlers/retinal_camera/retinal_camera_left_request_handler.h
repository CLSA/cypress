#ifndef RETINALCAMERALEFTREQUESTHANDLER_H
#define RETINALCAMERALEFTREQUESTHANDLER_H

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

#include "server/default_request_handler.h"

class RetinalCameraLeftRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;
};

#endif // RETINALCAMERALEFTREQUESTHANDLER_H
