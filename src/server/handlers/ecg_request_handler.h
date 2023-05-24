#ifndef ECG_REQUEST_HANDLER_H
#define ECG_REQUEST_HANDLER_H

#include "server/DefaultRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

class ECGRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // ECG_REQUEST_HANDLER_H
