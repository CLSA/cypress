#ifndef INSTRUMENTREQUESTHANDLERFACTORY_H
#define INSTRUMENTREQUESTHANDLERFACTORY_H

#include "Poco/Net/HTTPRequestHandlerFactory.h"

#include <QRegularExpression>
#include <QMap>

using namespace Poco::Net;
typedef HTTPRequestHandler* (*createRequestHandlerImpl)();

class InstrumentRequestHandlerFactory: public HTTPRequestHandlerFactory
{
public:
    InstrumentRequestHandlerFactory() = default;

    static HTTPRequestHandler* createGripStrengthHandler();
    static HTTPRequestHandler* createRetinalCameraHandler();
    static HTTPRequestHandler* createDXAHandler();
    static HTTPRequestHandler* createUltrasoundHandler();

    static QMap<QString, createRequestHandlerImpl> urlMap;

    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request);
};




#endif // INSTRUMENTREQUESTHANDLERFACTORY_H
