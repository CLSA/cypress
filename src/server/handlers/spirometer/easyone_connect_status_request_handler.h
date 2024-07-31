#ifndef EASYONE_CONNECT_STATUS_REQUEST_HANDLER_H
#define EASYONE_CONNECT_STATUS_REQUEST_HANDLER_H

#include "server/default_request_handler.h"

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

#include <QJsonObject>
#include <QDebug>

class EasyoneConnectStatusRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // EASYONE_CONNECT_STATUS_REQUEST_HANDLER_H
