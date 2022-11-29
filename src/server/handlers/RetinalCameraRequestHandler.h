#ifndef RETINALCAMERAREQUESTHANDLER_H
#define RETINALCAMERAREQUESTHANDLER_H

#include <Poco/Net/HTTPRequestHandler.h>
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

using namespace Poco::Net;

class RetinalCameraRequestHandler : public Poco::Net::HTTPRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // RETINALCAMERAREQUESTHANDLER_H
