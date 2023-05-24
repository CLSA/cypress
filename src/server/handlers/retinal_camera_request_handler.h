#ifndef RETINAL_CAMERA_REQUEST_HANDLER_H
#define RETINAL_CAMERA_REQUEST_HANDLER_H

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

#include "server/DefaultRequestHandler.h"

using namespace Poco::Net;

class RetinalCameraRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) override;
};

#endif // RETINAL_CAMERA_REQUEST_HANDLER_H
