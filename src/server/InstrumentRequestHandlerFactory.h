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

    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request);

    static QMap<QString, createRequestHandlerImpl> urlMap;

    static HTTPRequestHandler* defaultDeleteSessionRequestHandler();

    static HTTPRequestHandler* createGripStrengthRequestHandler();
    static HTTPRequestHandler* createGripStrengthStatusRequestHandler();

    static HTTPRequestHandler* createRetinalCameraRequestHandler();
    static HTTPRequestHandler* createRetinalCameraStatusRequestHandler();

    static HTTPRequestHandler* createAudiometerRequestHandler();
    static HTTPRequestHandler* createAudiometerStatusRequestHandler();

    static HTTPRequestHandler* createBloodPressureRequestHandler();
    static HTTPRequestHandler* createBloodPressureStatusRequestHandler();

    static HTTPRequestHandler* createDxaRequestHandler();
    static HTTPRequestHandler* createDxaStatusRequestHandler();

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

    static HTTPRequestHandler* createSignaturePadRequestHandler();
    static HTTPRequestHandler* createSignaturePadStatusRequestHandler();
};




#endif // INSTRUMENTREQUESTHANDLERFACTORY_H
