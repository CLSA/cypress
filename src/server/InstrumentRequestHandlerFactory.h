#ifndef INSTRUMENTREQUESTHANDLERFACTORY_H
#define INSTRUMENTREQUESTHANDLERFACTORY_H

#include <QRegularExpression>
#include <QMap>

#include "Poco/Net/HTTPRequestHandlerFactory.h"

using namespace Poco::Net;

typedef HTTPRequestHandler* (*createRequestHandlerImpl)();

class InstrumentRequestHandlerFactory: public Poco::Net::HTTPRequestHandlerFactory
{
public:
    InstrumentRequestHandlerFactory() = default;

    static QMap<QString, createRequestHandlerImpl> urlMap;

    static HTTPRequestHandler* createGripStrengthRequestHandler();
    static HTTPRequestHandler* createRetinalCameraRequestHandler();

    static HTTPRequestHandler* createAudiometerRequestHandler();
    static HTTPRequestHandler* createBloodPressureRequestHandler();
    static HTTPRequestHandler* createDXAForearmRequestHandler();
    static HTTPRequestHandler* createDXAHipRequestHandler();
    static HTTPRequestHandler* createDXASpineRequestHandler();
    static HTTPRequestHandler* createDXAIvaRequestHandler();
    static HTTPRequestHandler* createDXAWholeBodyRequestHandler();

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

    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request);
};




#endif // INSTRUMENTREQUESTHANDLERFACTORY_H
