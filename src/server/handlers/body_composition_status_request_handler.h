#ifndef BODYCOMPOSITIONSTATUSREQUESTHANDLER_H
#define BODYCOMPOSITIONSTATUSREQUESTHANDLER_H

#include <QJsonObject>
#include <QDebug>

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

#include "server/DefaultRequestHandler.h"

class BodyCompositionStatusRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // BODYCOMPOSITIONSTATUSREQUESTHANDLER_H
