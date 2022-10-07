#ifndef INSTRUMENTREQUESTHANDLERFACTORY_H
#define INSTRUMENTREQUESTHANDLERFACTORY_H

#include "Poco/Net/HTTPRequestHandlerFactory.h"


class InstrumentRequestHandlerFactory: public Poco::Net::HTTPRequestHandlerFactory
{
public:
    InstrumentRequestHandlerFactory();
    ~InstrumentRequestHandlerFactory();

    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request);
};

#endif // INSTRUMENTREQUESTHANDLERFACTORY_H
