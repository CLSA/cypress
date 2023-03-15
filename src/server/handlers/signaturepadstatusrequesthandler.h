#ifndef SIGNATUREPADSTATUSREQUESTHANDLER_H
#define SIGNATUREPADSTATUSREQUESTHANDLER_H

#include "server/DefaultRequestHandler.h"

class SignaturePadStatusRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // SIGNATUREPADSTATUSREQUESTHANDLER_H
