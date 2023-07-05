#ifndef AUDIOMETERSTATUSREQUESTHANDLER_H
#define AUDIOMETERSTATUSREQUESTHANDLER_H

#include "server/default_request_handler.h"

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

#include <QJsonObject>
#include <QDebug>


class AudiometerStatusRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // AUDIOMETERSTATUSREQUESTHANDLER_H
