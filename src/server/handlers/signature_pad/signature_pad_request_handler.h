#ifndef SIGNATURE_PAD_REQUEST_HANDLER_H
#define SIGNATURE_PAD_REQUEST_HANDLER_H

#include "server/default_request_handler.h"

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

class SignaturePadRequestHandler: public DefaultRequestHandler
{
public:
    virtual void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response);
    bool isValidInputData(const QJsonObject& inputData) override;
};

#endif // SIGNATURE_PAD_REQUEST_HANDLER_H
