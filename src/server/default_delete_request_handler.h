#ifndef DEFAULT_DELETE_REQUEST_HANDLER_H
#define DEFAULT_DELETE_REQUEST_HANDLER_H

#include "Poco/Net/HTTPRequestHandler.h"
#include <QJsonObject>

class DefaultDeleteSessionRequestHandler : public Poco::Net::HTTPRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // DEFAULT_DELETE_REQUEST_HANDLER_H
