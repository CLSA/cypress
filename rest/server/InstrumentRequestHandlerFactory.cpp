#include <iostream>
#include <string>

#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDebug>
#include <QMap>

#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"

#include "DefaultRequestHandler.h"
#include "InstrumentRequestHandlerFactory.h"
#include "GripStrengthRequestHandler.h"

using namespace Poco::Net;

InstrumentRequestHandlerFactory::InstrumentRequestHandlerFactory() {}
InstrumentRequestHandlerFactory::~InstrumentRequestHandlerFactory() {}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createRequestHandler(const HTTPServerRequest &request)
{
    // Iterate through the URL map and match the URI with a defined regex, returning the appropriate handler,
    // if there is no match, return the default (404) request handler.
    //

    QString uri = QString::fromStdString(request.getURI());
    QRegularExpressionMatch match;
    QMap<QString, createRequestHandlerImpl>::const_iterator i = urlMap.constBegin();
    while (i != urlMap.constEnd())
    {
        QString regexStr = i.key();
        QRegularExpression regex(regexStr);
        createRequestHandlerImpl requestHandlerFactoryFunc = i.value();

        match = regex.match(uri);
        if (match.hasMatch())
        {
            return requestHandlerFactoryFunc();
        }

        ++i;
    }

    return new DefaultRequestHandler;
}


HTTPRequestHandler* InstrumentRequestHandlerFactory::createGripStrengthHandler()
{
   return new GripStrengthRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createRetinalCameraHandler()
{
   return new GripStrengthRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createUltrasoundHandler()
{
   return new GripStrengthRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createDXAHandler()
{
   return new GripStrengthRequestHandler;
}


