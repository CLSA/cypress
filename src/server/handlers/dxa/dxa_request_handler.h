#ifndef DXA_REQUEST_HANDLER_H
#define DXA_REQUEST_HANDLER_H

#include "server/default_request_handler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

class DxaRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
    bool isValidInputData(const QJsonObject& inputData) override;
};

#endif // DXA_REQUEST_HANDLER_H
