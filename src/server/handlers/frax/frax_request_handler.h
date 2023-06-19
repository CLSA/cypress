#ifndef FRAX_REQUEST_HANDLER_H
#define FRAX_REQUEST_HANDLER_H

#include "server/default_request_handler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"


class FraxRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
    bool isValidInputData(const QJsonObject& inputData) override;
};

#endif // FRAX_REQUEST_HANDLER_H
