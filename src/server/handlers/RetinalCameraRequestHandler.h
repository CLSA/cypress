#ifndef RETINALCAMERAREQUESTHANDLER_H
#define RETINALCAMERAREQUESTHANDLER_H

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

#include "server/DefaultRequestHandler.h"

using namespace Poco::Net;

class RetinalCameraRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) override;
};

#endif // RETINALCAMERAREQUESTHANDLER_H
