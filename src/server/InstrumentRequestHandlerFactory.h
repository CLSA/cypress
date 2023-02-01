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

    static HTTPRequestHandler* createCypressStatusRequestHandler();
    static HTTPRequestHandler* createGripStrengthRequestHandler();
    static HTTPRequestHandler* createGripStrengthStatusRequestHandler();

    static HTTPRequestHandler* createRetinalCameraRequestHandler();
    static HTTPRequestHandler* createRetinalCameraStatusRequestHandler();

    static HTTPRequestHandler* createAudiometerRequestHandler();
    static HTTPRequestHandler* createAudiometerStatusRequestHandler();

    static HTTPRequestHandler* createBloodPressureRequestHandler();
    static HTTPRequestHandler* createBloodPressureStatusRequestHandler();

    static HTTPRequestHandler* createDXAForearmRequestHandler();
    static HTTPRequestHandler* createDXAForearmStatusRequestHandler();

    static HTTPRequestHandler* createDXAHipRequestHandler();
    static HTTPRequestHandler* createDXAHipStatusRequestHandler();

    static HTTPRequestHandler* createDXASpineRequestHandler();
    static HTTPRequestHandler* createDXASpineStatusRequestHandler();

    static HTTPRequestHandler* createDXAIvaRequestHandler();
    static HTTPRequestHandler* createDXAIvaStatusRequestHandler();

    static HTTPRequestHandler* createDXAWholeBodyRequestHandler();
    static HTTPRequestHandler* createDXAWholeBodyStatusRequestHandler();

    static HTTPRequestHandler* createUltrasoundRequestHandler();
    static HTTPRequestHandler* createUltrasoundStatusRequestHandler();

    static HTTPRequestHandler* createSpirometerRequestHandler();
    static HTTPRequestHandler* createSpirometerStatusRequestHandler();

    static HTTPRequestHandler* createEcgRequestHandler();
    static HTTPRequestHandler* createEcgStatusRequestHandler();

    static HTTPRequestHandler* createFraxRequestHandler();
    static HTTPRequestHandler* createFraxStatusRequestHandler();

    static HTTPRequestHandler* createTonometerRequestHandler();
    static HTTPRequestHandler* createTonometerStatusRequestHandler();

    static HTTPRequestHandler* createCDTTRequestHandler();
    static HTTPRequestHandler* createCDTTStatusRequestHandler();

    static HTTPRequestHandler* createEMRRequestHandler();
    static HTTPRequestHandler* createEMRStatusRequestHandler();

    static HTTPRequestHandler* createChoiceReactionRequestHandler();
    static HTTPRequestHandler* createChoiceReactionStatusRequestHandler();

    static HTTPRequestHandler* createWeighScaleRequestHandler();
    static HTTPRequestHandler* createWeighScaleStatusRequestHandler();

    static HTTPRequestHandler* createBodyCompositionRequestHandler();
    static HTTPRequestHandler* createBodyCompositionStatusRequestHandler();

    static HTTPRequestHandler* createECGRequestHandler();
    static HTTPRequestHandler* createECGStatusRequestHandler();

    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request);
};




#endif // INSTRUMENTREQUESTHANDLERFACTORY_H
