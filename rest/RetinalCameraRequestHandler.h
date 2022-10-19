#ifndef RETINALCAMERAREQUESTHANDLER_H
#define RETINALCAMERAREQUESTHANDLER_H

#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

using namespace Poco::Net;

class RetinalCameraRequestHandler: public HTTPRequestHandler
{
public:
    RetinalCameraRequestHandler();
    ~RetinalCameraRequestHandler();

    void handleRequest(HTTPServerRequest &request, HTTPServerResponse &response);
};

#endif // RETINALCAMERAREQUESTHANDLER_H
