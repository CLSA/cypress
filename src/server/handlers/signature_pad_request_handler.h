#ifndef SIGNATURE_PAD_REQUEST_HANDLER_H
#define SIGNATURE_PAD_REQUEST_HANDLER_H

#include "../DefaultRequestHandler.h"

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "CypressApplication.h"
#include "auxiliary/JsonSettings.h"

class SignaturePadRequestHandler: public DefaultRequestHandler
{
public:
    virtual void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response);
};

#endif // SIGNATURE_PAD_REQUEST_HANDLER_H
