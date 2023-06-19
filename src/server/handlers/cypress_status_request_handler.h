#ifndef CYPRESS_STATUS_REQUEST_HANDLER_H
#define CYPRESS_STATUS_REQUEST_HANDLER_H

#include <QJsonObject>
#include <QDebug>

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

#include "server/default_request_handler.h"

class CypressStatusRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // CYPRESS_STATUS_REQUEST_HANDLER_H
