#ifndef CDTT_STATUS_REQUEST_HANDLER_H
#define CDTT_STATUS_REQUEST_HANDLER_H

#include <QJsonObject>
#include <QDebug>

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

#include "server/default_request_handler.h"

class CDTTStatusRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // CDTT_STATUS_REQUEST_HANDLER_H
