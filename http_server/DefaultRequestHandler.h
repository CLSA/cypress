#ifndef DEFAULTREQUESTHANDLER_H
#define DEFAULTREQUESTHANDLER_H

#include "Poco/Net/HTTPRequestHandler.h"

class DefaultRequestHandler : public Poco::Net::HTTPRequestHandler
{
public:
    DefaultRequestHandler();
    ~DefaultRequestHandler();
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response);
};

#endif // DEFAULTREQUESTHANDLER_H
