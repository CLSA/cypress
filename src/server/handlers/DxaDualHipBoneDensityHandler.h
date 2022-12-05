#ifndef DXADUALHIPBONEDENSITYHANDLER_H
#define DXADUALHIPBONEDENSITYHANDLER_H

#include "server/DefaultRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"


class DxaDualHipBoneDensityHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // DXADUALHIPBONEDENSITYHANDLER_H
