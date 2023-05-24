#ifndef SPIROMETER_STATUS_REQUEST_HANDLER_H
#define SPIROMETER_STATUS_REQUEST_HANDLER_H


#include <QJsonObject>
#include <QDebug>

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

#include "server/DefaultRequestHandler.h"


class SpirometerStatusRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // SPIROMETER_STATUS_REQUEST_HANDLER_H
