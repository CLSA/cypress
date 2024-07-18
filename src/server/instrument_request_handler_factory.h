#ifndef INSTRUMENT_REQUEST_HANDLER_FACTORY_H
#define INSTRUMENT_REQUEST_HANDLER_FACTORY_H

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

    static HTTPRequestHandler* createRetinalCameraLeftRequestHandler();
    static HTTPRequestHandler* createRetinalCameraRightRequestHandler();

    static HTTPRequestHandler* createAudiometerRequestHandler();
    static HTTPRequestHandler* createAudiometerStatusRequestHandler();

    static HTTPRequestHandler* createHearconRequestHandler();
    static HTTPRequestHandler* createHearconStatusRequestHandler();

    static HTTPRequestHandler* createBloodPressureRequestHandler();
    static HTTPRequestHandler* createBloodPressureStatusRequestHandler();

    static HTTPRequestHandler* createWatchBPRequestHandler();
    static HTTPRequestHandler* createWatchBPStatusRequestHandler();

    static HTTPRequestHandler* createDxaHipRequestHandler();
    static HTTPRequestHandler* createDxaHipStatusRequestHandler();

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

    //static HTTPRequestHandler* createSignaturePadRequestHandler();
    //static HTTPRequestHandler* createSignaturePadStatusRequestHandler();

    static HTTPRequestHandler* createParticipantReportRequestHandler();
    static HTTPRequestHandler* createGeneralProxyConsentRequestHandler();
    static HTTPRequestHandler* createGeneralProxyConsentStatusRequestHandler();
    static HTTPRequestHandler* createDxaHipSessionRequestHandler();
    static HTTPRequestHandler* createDxaSessionRequestHandler();

    static HTTPRequestHandler* createOCTSessionRequestHandler();
    static HTTPRequestHandler* createOCTStatusRequestHandler();
};

#endif // INSTRUMENT_REQUEST_HANDLER_FACTORY_H
