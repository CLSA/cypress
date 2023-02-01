#ifndef DXAWHOLEBODYSTATUSREQUESTHANDLER_H
#define DXAWHOLEBODYSTATUSREQUESTHANDLER_H

#include <QJsonObject>
#include <QDebug>

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

#include "server/DefaultRequestHandler.h"

class DXAWholeBodyStatusRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // DXAWHOLEBODYSTATUSREQUESTHANDLER_H
