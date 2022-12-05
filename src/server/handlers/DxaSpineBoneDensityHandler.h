#ifndef DXASPINEBONEDENSITYHANDLER_H
#define DXASPINEBONEDENSITYHANDLER_H

#include "server/DefaultRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

class DxaSpineBoneDensityHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // DXASPINEBONEDENSITYHANDLER_H
