#ifndef INSTRUMENTREQUESTHANDLERFACTORY_H
#define INSTRUMENTREQUESTHANDLERFACTORY_H

#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include <QRegularExpression>
#include <QMap>

using namespace Poco::Net;


typedef HTTPRequestHandler* (*createRequestHandlerImpl)();



class InstrumentRequestHandlerFactory: public Poco::Net::HTTPRequestHandlerFactory
{
public:
    InstrumentRequestHandlerFactory() = default;

    static HTTPRequestHandler* createGripStrengthRequestHandler();
    static HTTPRequestHandler* createRetinalCameraRequestHandler();
    static HTTPRequestHandler* createDXARequestHandler();
    static HTTPRequestHandler* createUltrasoundRequestHandler();
    static HTTPRequestHandler* createSpirometerRequestHandler();
    static HTTPRequestHandler* createEcgRequestHandler();
    static HTTPRequestHandler* createFraxRequestHandler();
    static HTTPRequestHandler* createTonometerRequestHandler();
    static HTTPRequestHandler* createCDDTRequestHandler();
    static HTTPRequestHandler* createEMRRequestHandler();
    static HTTPRequestHandler* createChoiceReactionRequestHandler();
    static HTTPRequestHandler* createWeighScaleRequestHandler();
    static HTTPRequestHandler* createBodyCompositionRequestHandler();
    static HTTPRequestHandler* createECGRequestHandler();

    static QMap<QString, createRequestHandlerImpl> urlMap;

    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request);
};




#endif // INSTRUMENTREQUESTHANDLERFACTORY_H
