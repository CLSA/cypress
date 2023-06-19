#ifndef RETINAL_CAMERA_REQUEST_HANDLER_H
#define RETINAL_CAMERA_REQUEST_HANDLER_H

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

#include "server/default_request_handler.h"

using namespace Poco::Net;

class RetinalCameraRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) override;
    bool isValidInputData(const QJsonObject& inputData) override;
};

#endif // RETINAL_CAMERA_REQUEST_HANDLER_H
