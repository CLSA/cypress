#ifndef DXAFOREARMBONEDENSITYHANDLER_H
#define DXAFOREARMBONEDENSITYHANDLER_H

#include "server/DefaultRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"


class DxaForearmBoneDensityHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // DXAFOREARMBONEDENSITYHANDLER_H
