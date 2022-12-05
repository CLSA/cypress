#ifndef DXAWHOLEBODYBONEDENSITYHANDLER_H
#define DXAWHOLEBODYBONEDENSITYHANDLER_H

#include "server/DefaultRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

class DxaWholeBodyBoneDensityHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // DXAWHOLEBODYBONEDENSITYHANDLER_H
