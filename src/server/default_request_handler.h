#ifndef DEFAULT_REQUEST_HANDLER_H
#define DEFAULT_REQUEST_HANDLER_H

#include "Poco/Net/HTTPRequestHandler.h"
#include <QJsonObject>

class DefaultRequestHandler : public Poco::Net::HTTPRequestHandler
{
public:
    virtual void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response);

protected:
    virtual bool isBusy();
    virtual bool isValidInputData(const QJsonObject& inputData);

    virtual QJsonObject getResponseData();
    virtual QJsonObject getRequestData(Poco::Net::HTTPServerRequest &request);
};

#endif // DEFAULT_REQUEST_HANDLER_H
