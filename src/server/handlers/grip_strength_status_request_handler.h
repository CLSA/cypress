#ifndef GRIP_STRENGTH_STATUS_REQUEST_HANDLER_H
#define GRIP_STRENGTH_STATUS_REQUEST_HANDLER_H

#include <QJsonObject>
#include <QDebug>

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

#include "server/DefaultRequestHandler.h"

class GripStrengthStatusRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // GRIP_STRENGTH_STATUS_REQUEST_HANDLER_H
