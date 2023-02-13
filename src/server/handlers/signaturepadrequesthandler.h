#ifndef SIGNATUREPADREQUESTHANDLER_H
#define SIGNATUREPADREQUESTHANDLER_H

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

#endif // SIGNATUREPADREQUESTHANDLER_H
