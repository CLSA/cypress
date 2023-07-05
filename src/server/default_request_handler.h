#ifndef DEFAULT_REQUEST_HANDLER_H
#define DEFAULT_REQUEST_HANDLER_H

#include "cypress_session.h"
#include "Poco/Net/HTTPRequestHandler.h"

#include <QJsonObject>

class DefaultRequestHandler : public Poco::Net::HTTPRequestHandler
{
public:
    virtual void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response);

protected:
    virtual QJsonObject getRequestData(Poco::Net::HTTPServerRequest &request);
    virtual QJsonObject getResponseData(const QString& sessionId);
};

#endif // DEFAULT_REQUEST_HANDLER_H
