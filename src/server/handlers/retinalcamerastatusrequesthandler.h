#ifndef RETINALCAMERASTATUSREQUESTHANDLER_H
#define RETINALCAMERASTATUSREQUESTHANDLER_H


#include <QJsonObject>
#include <QDebug>

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

#include "server/DefaultRequestHandler.h"


class RetinalCameraStatusRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // RETINALCAMERASTATUSREQUESTHANDLER_H