#ifndef DEFAULTDELETESESSIONREQUESTHANDLER_H
#define DEFAULTDELETESESSIONREQUESTHANDLER_H

#include "Poco/Net/HTTPRequestHandler.h"
#include <QJsonObject>

class DefaultDeleteSessionRequestHandler : public Poco::Net::HTTPRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // DEFAULTDELETESESSIONREQUESTHANDLER_H
