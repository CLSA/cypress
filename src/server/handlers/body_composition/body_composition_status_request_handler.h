#ifndef BODYCOMPOSITIONSTATUSREQUESTHANDLER_H
#define BODYCOMPOSITIONSTATUSREQUESTHANDLER_H

#include "server/default_request_handler.h"

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

#include <QJsonObject>
#include <QDebug>

class BodyCompositionStatusRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // BODYCOMPOSITIONSTATUSREQUESTHANDLER_H
