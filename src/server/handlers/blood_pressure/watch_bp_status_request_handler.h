#ifndef WATCH_BP_STATUS_REQUEST_HANDLER_H
#define WATCH_BP_STATUS_REQUEST_HANDLER_H

#include "server/default_request_handler.h"

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

#include <QJsonObject>
#include <QDebug>

class WatchBPStatusRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // WATCH_BP_STATUS_REQUEST_HANDLER_H
