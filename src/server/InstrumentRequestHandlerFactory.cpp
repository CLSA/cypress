#include "InstrumentRequestHandlerFactory.h"

#include <QDebug>
#include <QString>
#include <iostream>
#include "GripStrengthRequestHandler.h"

#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"

InstrumentRequestHandlerFactory::InstrumentRequestHandlerFactory()
{

}

InstrumentRequestHandlerFactory::~InstrumentRequestHandlerFactory()
{

}

Poco::Net::HTTPRequestHandler* InstrumentRequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest &request)
{
    return new GripStrengthRequestHandler;
}
