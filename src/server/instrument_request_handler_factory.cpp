#include "server/handlers/grip_strength/grip_strength_request_handler.h"
#include "server/handlers/grip_strength/grip_strength_status_request_handler.h"

#include "server/handlers/retinal_camera/retinal_camera_left_request_handler.h"
#include "server/handlers/retinal_camera/retinal_camera_right_request_handler.h"
#include "server/handlers/retinal_camera/retinal_camera_status_request_handler.h"

#include "server/handlers/blood_pressure/blood_pressure_request_handler.h"
#include "server/handlers/blood_pressure/blood_pressure_status_request_handler.h"

#include "server/handlers/blood_pressure/watch_bp_request_handler.h"
#include "server/handlers/blood_pressure/watch_bp_status_request_handler.h"

#include "server/handlers/audiometer/audiometer_request_handler.h"
#include "server/handlers/audiometer/audiometer_status_request_handler.h"

#include "server/handlers/audiometer/hearcon_request_handler.h"
#include "server/handlers/audiometer/hearcon_status_request_handler.h"

#include "server/handlers/cdtt/cdtt_request_handler.h"
#include "server/handlers/cdtt/cdtt_status_request_handler.h"

#include "server/handlers/choice_reaction/choice_reaction_request_handler.h"
#include "server/handlers/choice_reaction/choice_reaction_status_request_handler.h"

#include "server/handlers/dxa/dxa_hip_request_handler.h"
#include "server/handlers/dxa/dxa_hip_status_request_handler.h"

#include "server/handlers/dxa/dxa_request_handler.h"
#include "server/handlers/dxa/dxa_status_request_handler.h"

#include "server/handlers/ecg/ecg_request_handler.h"
#include "server/handlers/ecg/ecg_status_request_handler.h"

#include "server/handlers/frax/frax_request_handler.h"
#include "server/handlers/frax/frax_status_request_handler.h"

#include "server/handlers/spirometer/spirometer_request_handler.h"
#include "server/handlers/spirometer/spirometer_status_request_handler.h"

#include "server/handlers/spirometer/easyone_connect_request_handler.h"
#include "server/handlers/spirometer/easyone_connect_status_request_handler.h"

#include "server/handlers/tonometer/tonometer_request_handler.h"
#include "server/handlers/tonometer/tonometer_status_request_handler.h"

#include "server/handlers/tonometer/ora_request_handler.h"
#include "server/handlers/tonometer/ora_status_request_handler.h"

#include "server/handlers/weigh_scale/weigh_scale_request_handler.h"
#include "server/handlers/weigh_scale/weigh_scale_status_request_handler.h"

#include "server/handlers/body_composition/body_composition_request_handler.h"
#include "server/handlers/body_composition/body_composition_status_request_handler.h"

#include "server/handlers/ultrasound/ultrasound_request_handler.h"
#include "server/handlers/ultrasound/ultrasound_status_request_handler.h"

#include "server/handlers/general_proxy_consent/general_proxy_consent_request_handler.h"
#include "server/handlers/general_proxy_consent/general_proxy_consent_status_request_handler.h"

#include "server/handlers/participant_report/participant_report_request_handler.h"

#include "server/handlers/dxa/dxa_hip_session_request_handler.h"
#include "server/handlers/dxa/dxa_session_request_handler.h"

#include "server/handlers/oct_request_handler.h"
#include "server/handlers/oct_status_request_handler.h"

#include "server/default_request_handler.h"
#include "server/default_delete_request_handler.h"

#include "server/instrument_request_handler_factory.h"

#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDebug>
#include <QMap>

#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"

using namespace Poco::Net;

QMap<QString, createRequestHandlerImpl> InstrumentRequestHandlerFactory::urlMap = {{
    { QString(R"(^/audiometer/?$)"),        &InstrumentRequestHandlerFactory::createAudiometerRequestHandler },
    { QString(R"(^/audiometer/status/?$)"), &InstrumentRequestHandlerFactory::createAudiometerStatusRequestHandler },
    { QString(R"(^/audiometer/delete/?$)"), &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler },

    { QString(R"(^/hearcon/?$)"),         &InstrumentRequestHandlerFactory::createHearconRequestHandler},
    { QString(R"(^/hearcon/status/?$")"), &InstrumentRequestHandlerFactory::createHearconStatusRequestHandler},
    { QString(R"(^/hearcon/delete/?$)"),  &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler},

    { QString(R"(^/blood_pressure/?$)"),        &InstrumentRequestHandlerFactory::createBloodPressureRequestHandler },
    { QString(R"(^/blood_pressure/status/?$)"), &InstrumentRequestHandlerFactory::createBloodPressureStatusRequestHandler },
    { QString(R"(^/blood_pressure/delete/?$)"), &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler },

    { QString(R"(^/watch_bp/?$)"),        &InstrumentRequestHandlerFactory::createWatchBPRequestHandler },
    { QString(R"(^/watch_bp/status/?$)"), &InstrumentRequestHandlerFactory::createWatchBPStatusRequestHandler },
    { QString(R"(^/watch_bp/delete/?$)"), &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler },

    { QString(R"(^/cdtt/?$)"),        &InstrumentRequestHandlerFactory::createCDTTRequestHandler },
    { QString(R"(^/cdtt/status/?$)"), &InstrumentRequestHandlerFactory::createCDTTStatusRequestHandler },
    { QString(R"(^/cdtt/delete/?$)"), &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler },

    { QString(R"(^/choice_reaction_test/?$)"),        &InstrumentRequestHandlerFactory::createChoiceReactionRequestHandler },
    { QString(R"(^/choice_reaction_test/status/?$)"), &InstrumentRequestHandlerFactory::createChoiceReactionStatusRequestHandler },
    { QString(R"(^/choice_reaction_test/delete/?$)"), &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler },

    { QString(R"(^/dxa1/?$)"),          &InstrumentRequestHandlerFactory::createDxaHipRequestHandler },
    { QString(R"(^/dxa1/status/?$)"),   &InstrumentRequestHandlerFactory::createDxaHipStatusRequestHandler },
    { QString(R"(^/dxa1/session/?$)"),  &InstrumentRequestHandlerFactory::createDxaHipSessionRequestHandler },
    { QString(R"(^/dxa1/delete/?$)"),   &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler },

    { QString(R"(^/dxa2/?$)"),         &InstrumentRequestHandlerFactory::createDxaRequestHandler },
    { QString(R"(^/dxa2/status/?$)"),  &InstrumentRequestHandlerFactory::createDxaStatusRequestHandler },
    { QString(R"(^/dxa2/session/?$)"), &InstrumentRequestHandlerFactory::createDxaSessionRequestHandler },
    { QString(R"(^/dxa2/delete/?$)"),  &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler },

    { QString(R"(^/ecg/?$)"),        &InstrumentRequestHandlerFactory::createECGRequestHandler },
    { QString(R"(^/ecg/status/?$)"), &InstrumentRequestHandlerFactory::createECGStatusRequestHandler },
    { QString(R"(^/ecg/delete/?$)"), &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler },

    { QString(R"(^/frax/?$)"),        &InstrumentRequestHandlerFactory::createFraxRequestHandler },
    { QString(R"(^/frax/status/?$)"), &InstrumentRequestHandlerFactory::createFraxStatusRequestHandler },
    { QString(R"(^/frax/delete/?$)"), &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler },

    { QString(R"(^/hand_grip/?$)"),        &InstrumentRequestHandlerFactory::createGripStrengthRequestHandler },
    { QString(R"(^/hand_grip/status/?$)"), &InstrumentRequestHandlerFactory::createGripStrengthStatusRequestHandler },

    { QString(R"(^/hand_grip/delete/?$)"),    &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler },
    { QString(R"(^/retinal_camera_left/?$)"), &InstrumentRequestHandlerFactory::createRetinalCameraLeftRequestHandler },

    { QString(R"(^/retinal_camera_right/?$)"),       &InstrumentRequestHandlerFactory::createRetinalCameraRightRequestHandler },
    { QString(R"(^/retinal_camera_left/status/?$)"), &InstrumentRequestHandlerFactory::createRetinalCameraStatusRequestHandler },

    { QString(R"(^/retinal_camera_left/delete/?$)"),  &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler },
    { QString(R"(^/retinal_camera_right/status/?$)"), &InstrumentRequestHandlerFactory::createRetinalCameraStatusRequestHandler },
    { QString(R"(^/retinal_camera_right/delete/?$)"), &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler },

    { QString(R"(^/spirometer/?$)"),        &InstrumentRequestHandlerFactory::createSpirometerRequestHandler },
    { QString(R"(^/spirometer/status/?$)"), &InstrumentRequestHandlerFactory::createSpirometerStatusRequestHandler },
    { QString(R"(^/spirometer/delete/?$)"), &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler },

    { QString(R"(^/easyone_connect/?$)"),        &InstrumentRequestHandlerFactory::createEasyoneConnectRequestHandler},
    { QString(R"(^/easyone_connect/status/?$)"), &InstrumentRequestHandlerFactory::createEasyoneConnectStatusRequestHandler},
    { QString(R"(^/easyone_connect/delete/?$)"), &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler},

    { QString(R"(^/tonometer/?$)"),        &InstrumentRequestHandlerFactory::createTonometerRequestHandler},
    { QString(R"(^/tonometer/status/?$)"), &InstrumentRequestHandlerFactory::createTonometerStatusRequestHandler},
    { QString(R"(^/tonometer/delete/?$)"), &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler},

    { QString(R"(^/ora/?$)"),        &InstrumentRequestHandlerFactory::createORARequestHandler},
    { QString(R"(^/ora/status/?$)"), &InstrumentRequestHandlerFactory::createORAStatusRequestHandler},
    { QString(R"(^/ora/delete/?$)"), &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler},

    { QString(R"(^/carotid_intima/?$)"),        &InstrumentRequestHandlerFactory::createUltrasoundRequestHandler},
    { QString(R"(^/carotid_intima/status/?$)"), &InstrumentRequestHandlerFactory::createUltrasoundStatusRequestHandler},
    { QString(R"(^/carotid_intima/delete/?$)"), &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler},

    { QString(R"(^/weight_scale/?$)"),        &InstrumentRequestHandlerFactory::createWeighScaleRequestHandler },
    { QString(R"(^/weight_scale/status/?$)"), &InstrumentRequestHandlerFactory::createWeighScaleStatusRequestHandler },
    { QString(R"(^/weight_scale/delete/?$)"), &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler },

    { QString(R"(^/general_proxy_consent/?$)"), &InstrumentRequestHandlerFactory::createGeneralProxyConsentRequestHandler },
    { QString(R"(^/general_proxy_consent/status/?$)"), &InstrumentRequestHandlerFactory::createGeneralProxyConsentStatusRequestHandler },
    { QString(R"(^/general_proxy_consent/delete/?$)"), &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler },

    { QString(R"(^/oct/?$)"), &InstrumentRequestHandlerFactory::createOCTSessionRequestHandler},
    { QString(R"(^/oct/status/?$)"), &InstrumentRequestHandlerFactory::createOCTStatusRequestHandler}}
};

HTTPRequestHandler* InstrumentRequestHandlerFactory::createRequestHandler(const HTTPServerRequest &request)
{
    /*
     * Iterate through the URL map and attempt to match the request URI with a handler,
     * if there is no match, return the default request handler (404).
     *
     */
    QString uri               = QString::fromStdString(request.getURI());
    QString method            = QString::fromStdString(request.getMethod());

    QString host              = QString::fromStdString(request.getHost().c_str());
    QString clientAddress     = QString::fromStdString(request.clientAddress().host().toString());
    QString clientPort        = QString::number(request.clientAddress().port());

    QString origin            = QString::fromStdString(request.get("Origin", "N/A").c_str());

    qInfo() << uri;
    qInfo() << method;
    qInfo() << host;
    qInfo() << clientAddress;
    qInfo() << clientPort;
    qInfo() << origin;

    QRegularExpressionMatch match;
    QMap<QString, createRequestHandlerImpl>::const_iterator handlerIter = urlMap.constBegin();

    if (method == "DELETE")
        return defaultDeleteSessionRequestHandler();

    while (handlerIter != urlMap.constEnd()) {
        QRegularExpression regex(handlerIter.key());
        createRequestHandlerImpl requestHandlerFactoryFunc = handlerIter.value();

        match = regex.match(uri);
        if (match.hasMatch())
            return requestHandlerFactoryFunc();

        ++handlerIter;
    }

    return new DefaultRequestHandler;
}

HTTPRequestHandler *InstrumentRequestHandlerFactory::createDxaHipSessionRequestHandler()
{
    return new DxaHipSessionRequestHandler;
}

HTTPRequestHandler *InstrumentRequestHandlerFactory::createDxaSessionRequestHandler()
{
    return new DxaSessionRequestHandler;
}

HTTPRequestHandler *InstrumentRequestHandlerFactory::createOCTSessionRequestHandler()
{
    return new OCTRequestHandler;
}

HTTPRequestHandler *InstrumentRequestHandlerFactory::createOCTStatusRequestHandler()
{
    return new OCTStatusRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler()
{
    return new DefaultDeleteSessionRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createAudiometerRequestHandler()
{
   return new AudiometerRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createAudiometerStatusRequestHandler()
{
   return new AudiometerStatusRequestHandler;
}

HTTPRequestHandler *InstrumentRequestHandlerFactory::createHearconRequestHandler()
{
    return new HearconRequestHandler;
}

HTTPRequestHandler *InstrumentRequestHandlerFactory::createHearconStatusRequestHandler()
{
    return new HearconStatusRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createBloodPressureRequestHandler()
{
    return new BloodPressureRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createBloodPressureStatusRequestHandler()
{
    return new BloodPressureStatusRequestHandler;
}

HTTPRequestHandler *InstrumentRequestHandlerFactory::createWatchBPRequestHandler()
{
    return new WatchBPRequestHandler;
}

HTTPRequestHandler *InstrumentRequestHandlerFactory::createWatchBPStatusRequestHandler()
{
    return new WatchBPStatusRequestHandler;
}

HTTPRequestHandler *InstrumentRequestHandlerFactory::createDxaHipRequestHandler()
{
    return new DxaHipRequestHandler;
}

HTTPRequestHandler *InstrumentRequestHandlerFactory::createDxaHipStatusRequestHandler()
{
    return new DxaHipStatusRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createGripStrengthRequestHandler()
{
   return new GripStrengthRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createGripStrengthStatusRequestHandler()
{
   return new GripStrengthStatusRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createRetinalCameraLeftRequestHandler()
{
   return new RetinalCameraLeftRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createRetinalCameraRightRequestHandler()
{
   return new RetinalCameraRightRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createRetinalCameraStatusRequestHandler()
{
   return new RetinalCameraStatusRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createUltrasoundRequestHandler()
{
   return new UltrasoundRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createUltrasoundStatusRequestHandler()
{
   return new UltrasoundStatusRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createDxaRequestHandler()
{
   return new DxaRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createDxaStatusRequestHandler()
{
   return new DxaStatusRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createSpirometerRequestHandler()
{
    return new SpirometerRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createSpirometerStatusRequestHandler()
{
    return new SpirometerStatusRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createEasyoneConnectRequestHandler()
{
    return new EasyoneConnectRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createEasyoneConnectStatusRequestHandler()
{
    return new EasyoneConnectStatusRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createECGRequestHandler()
{
    return new ECGRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createECGStatusRequestHandler()
{
    return new ECGStatusRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createFraxRequestHandler()
{
    return new FraxRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createFraxStatusRequestHandler()
{
    return new FraxStatusRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createTonometerRequestHandler()
{
    return new TonometerRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createTonometerStatusRequestHandler()
{
    return new TonometerStatusRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createORARequestHandler()
{
    return new ORARequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createORAStatusRequestHandler()
{
    return new ORAStatusRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createCDTTRequestHandler()
{
    return new CDTTRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createCDTTStatusRequestHandler()
{
    return new CDTTStatusRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createEcgRequestHandler()
{
    return new ECGRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createEcgStatusRequestHandler()
{
    return new ECGStatusRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createChoiceReactionRequestHandler()
{
    return new ChoiceReactionRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createChoiceReactionStatusRequestHandler()
{
    return new ChoiceReactionStatusRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createWeighScaleRequestHandler()
{
    return new WeighScaleRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createWeighScaleStatusRequestHandler()
{
    return new WeighScaleStatusRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createBodyCompositionRequestHandler()
{
    return new BodyCompositionRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createBodyCompositionStatusRequestHandler()
{
    return new BodyCompositionStatusRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createParticipantReportRequestHandler()
{
    return new ParticipantReportRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createGeneralProxyConsentRequestHandler()
{
    return new GeneralProxyConsentRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createGeneralProxyConsentStatusRequestHandler()
{
    return new GeneralProxyConsentStatusRequestHandler;
}
