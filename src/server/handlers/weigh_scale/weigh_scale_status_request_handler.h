#ifndef WEIGH_SCALE_STATUS_REQUEST_HANDLER_H
#define WEIGH_SCALE_STATUS_REQUEST_HANDLER_H

#include "server/default_request_handler.h"

class WeighScaleStatusRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // WEIGH_SCALE_STATUS_REQUEST_HANDLER_H
