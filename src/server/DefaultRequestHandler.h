#ifndef DEFAULTREQUESTHANDLER_H
#define DEFAULTREQUESTHANDLER_H

#include "Poco/Net/HTTPRequestHandler.h"
#include <QJsonObject>

class DefaultRequestHandler : public Poco::Net::HTTPRequestHandler
{
public:
    virtual void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response);

protected:
    virtual QJsonObject getResponseData();
};

#endif // DEFAULTREQUESTHANDLER_H
