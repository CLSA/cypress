#ifndef WEIGHSCALESTATUSREQUESTHANDLER_H
#define WEIGHSCALESTATUSREQUESTHANDLER_H

#include <QJsonObject>
#include <QDebug>

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

#include "server/DefaultRequestHandler.h"


class WeighScaleStatusRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // WEIGHSCALESTATUSREQUESTHANDLER_H
