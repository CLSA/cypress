#ifndef DXALATERALBONEDENSITYHANDLER_H
#define DXALATERALBONEDENSITYHANDLER_H

#include "server/DefaultRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"


class DxaLateralBoneDensityHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // DXALATERALBONEDENSITYHANDLER_H
