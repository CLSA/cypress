#ifndef BLOODPRESSURESTATUSREQUESTHANDLER_H
#define BLOODPRESSURESTATUSREQUESTHANDLER_H

#include <QJsonObject>
#include <QDebug>

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

#include "server/default_request_handler.h"

class BloodPressureStatusRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // BLOODPRESSURESTATUSREQUESTHANDLER_H
