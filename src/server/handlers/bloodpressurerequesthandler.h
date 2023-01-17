#ifndef BLOODPRESSUREREQUESTHANDLER_H
#define BLOODPRESSUREREQUESTHANDLER_H

#include "server/DefaultRequestHandler.h"

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

class BloodPressureRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // BLOODPRESSUREREQUESTHANDLER_H
