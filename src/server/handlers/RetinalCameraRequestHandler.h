#ifndef RETINALCAMERAREQUESTHANDLER_H
#define RETINALCAMERAREQUESTHANDLER_H

#include <Poco/Net/HTTPRequestHandler.h>

using namespace Poco::Net;

class RetinalCameraRequestHandler : public HTTPRequestHandler
{
public:
    RetinalCameraRequestHandler();
};

#endif // RETINALCAMERAREQUESTHANDLER_H
