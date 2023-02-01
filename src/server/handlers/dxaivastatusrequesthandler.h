#ifndef DXAIVASTATUSREQUESTHANDLER_H
#define DXAIVASTATUSREQUESTHANDLER_H

#include <QJsonObject>
#include <QDebug>

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

#include "server/DefaultRequestHandler.h"

class DXAIvaStatusRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // DXAIVASTATUSREQUESTHANDLER_H
