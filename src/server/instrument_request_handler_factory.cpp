#include <iostream>

#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDebug>
#include <QMap>

#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"

#include "server/default_request_handler.h"
#include "server/default_delete_request_handler.h"

#include "server/instrument_request_handler_factory.h"

#include "server/handlers/grip_strength/grip_strength_request_handler.h"
#include "server/handlers/grip_strength/grip_strength_status_request_handler.h"

#include "server/handlers/retinal_camera/retinal_camera_left_request_handler.h"
#include "server/handlers/retinal_camera/retinal_camera_right_request_handler.h"
#include "server/handlers/retinal_camera/retinal_camera_status_request_handler.h"

#include "server/handlers/blood_pressure/blood_pressure_request_handler.h"
#include "server/handlers/blood_pressure/blood_pressure_status_request_handler.h"

#include "server/handlers/audiometer/audiometer_request_handler.h"
#include "server/handlers/audiometer/audiometer_status_request_handler.h"

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

#include "server/handlers/tonometer/tonometer_request_handler.h"
#include "server/handlers/tonometer/tonometer_status_request_handler.h"

#include "server/handlers/weigh_scale/weigh_scale_request_handler.h"
#include "server/handlers/weigh_scale/weigh_scale_status_request_handler.h"

#include "server/handlers/body_composition/body_composition_request_handler.h"
#include "server/handlers/body_composition/body_composition_status_request_handler.h"

#include "server/handlers/ultrasound/ultrasound_request_handler.h"
#include "server/handlers/ultrasound/ultrasound_status_request_handler.h"

//#include "server/handlers/signature_pad/signature_pad_request_handler.h"
//#include "server/handlers/signature_pad/signature_pad_status_request_handler.h"

#include "server/handlers/general_proxy_consent/general_proxy_consent_request_handler.h"
#include "server/handlers/participant_report/participant_report_request_handler.h"

using namespace Poco::Net;

QMap<QString, createRequestHandlerImpl> InstrumentRequestHandlerFactory::urlMap =
{{
    { QString(R"(^/audiometer/?$)"),                       &InstrumentRequestHandlerFactory::createAudiometerRequestHandler            },
    { QString(R"(^/audiometer/status/?$)"),                &InstrumentRequestHandlerFactory::createAudiometerStatusRequestHandler      },
    { QString(R"(^/audiometer/delete/?$)"),                &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler        },

    { QString(R"(^/blood_pressure/?$)"),                   &InstrumentRequestHandlerFactory::createBloodPressureRequestHandler         },
    { QString(R"(^/blood_pressure/status/?$)"),            &InstrumentRequestHandlerFactory::createBloodPressureStatusRequestHandler   },
    { QString(R"(^/blood_pressure/delete/?$)"),            &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler        },

    { QString(R"(^/body_composition/?$)"),                 &InstrumentRequestHandlerFactory::createBodyCompositionRequestHandler       },
    { QString(R"(^/body_composition/status/?$)"),          &InstrumentRequestHandlerFactory::createBodyCompositionStatusRequestHandler },
    { QString(R"(^/body_composition/delete/?$)"),          &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler        },

    { QString(R"(^/cdtt/?$)"),                             &InstrumentRequestHandlerFactory::createCDTTRequestHandler                  },
    { QString(R"(^/cdtt/status/?$)"),                      &InstrumentRequestHandlerFactory::createCDTTStatusRequestHandler            },
    { QString(R"(^/cdtt/delete/?$)"),                      &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler        },

    { QString(R"(^/choice_reaction_test/?$)"),             &InstrumentRequestHandlerFactory::createChoiceReactionRequestHandler        },
    { QString(R"(^/choice_reaction_test/status/?$)"),      &InstrumentRequestHandlerFactory::createChoiceReactionStatusRequestHandler  },
    { QString(R"(^/choice_reaction_test/delete/?$)"),      &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler        },

    { QString(R"(^/dxa1/?$)"),                             &InstrumentRequestHandlerFactory::createDxaHipRequestHandler                },
    { QString(R"(^/dxa1/status/?$)"),                      &InstrumentRequestHandlerFactory::createDxaHipStatusRequestHandler          },
    { QString(R"(^/dxa1/delete/?$)"),                      &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler        },

    { QString(R"(^/dxa2/?$)"),                             &InstrumentRequestHandlerFactory::createDxaRequestHandler                   },
    { QString(R"(^/dxa2/status/?$)"),                      &InstrumentRequestHandlerFactory::createDxaStatusRequestHandler             },
    { QString(R"(^/dxa2/delete/?$)"),                      &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler        },

    { QString(R"(^/ecg/?$)"), 	                           &InstrumentRequestHandlerFactory::createECGRequestHandler                   },
    { QString(R"(^/ecg/status/?$)"),                       &InstrumentRequestHandlerFactory::createECGStatusRequestHandler             },
    { QString(R"(^/ecg/delete/?$)"),                       &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler        },

    { QString(R"(^/frax/?$)"), 	                           &InstrumentRequestHandlerFactory::createFraxRequestHandler 			       },
    { QString(R"(^/frax/status/?$)"), 	                   &InstrumentRequestHandlerFactory::createFraxStatusRequestHandler 		   },
    { QString(R"(^/frax/delete/?$)"),                      &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler        },

    { QString(R"(^/hand_grip/?$)"),                        &InstrumentRequestHandlerFactory::createGripStrengthRequestHandler  	       },
    { QString(R"(^/hand_grip/status/?$)"),                 &InstrumentRequestHandlerFactory::createGripStrengthStatusRequestHandler    },
    { QString(R"(^/hand_grip/delete/?$)"),                 &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler        },

    { QString(R"(^/retinal_camera_left/?$)"),          	   &InstrumentRequestHandlerFactory::createRetinalCameraLeftRequestHandler     },
    { QString(R"(^/retinal_camera_right/?$)"),             &InstrumentRequestHandlerFactory::createRetinalCameraRightRequestHandler    },

    { QString(R"(^/retinal_camera_left/status/?$)"),       &InstrumentRequestHandlerFactory::createRetinalCameraStatusRequestHandler   },
    { QString(R"(^/retinal_camera_left/delete/?$)"),       &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler        },

    { QString(R"(^/retinal_camera_right/status/?$)"),      &InstrumentRequestHandlerFactory::createRetinalCameraStatusRequestHandler   },
    { QString(R"(^/retinal_camera_right/delete/?$)"),      &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler        },

    { QString(R"(^/spirometer/?$)"),                       &InstrumentRequestHandlerFactory::createSpirometerRequestHandler            },
    { QString(R"(^/spirometer/status/?$)"),                &InstrumentRequestHandlerFactory::createSpirometerStatusRequestHandler      },
    { QString(R"(^/spirometer/delete/?$)"),                &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler        },

    { QString(R"(^/tonometer/?$)"),	                       &InstrumentRequestHandlerFactory::createTonometerRequestHandler		       },
    { QString(R"(^/tonometer/status/?$)"),                 &InstrumentRequestHandlerFactory::createTonometerStatusRequestHandler	   },
    { QString(R"(^/tonometer/delete/?$)"),                 &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler        },

    { QString(R"(^/carotid_intima/?$)"), 	   	           &InstrumentRequestHandlerFactory::createUltrasoundRequestHandler            },
    { QString(R"(^/carotid_intima/status/?$)"),            &InstrumentRequestHandlerFactory::createUltrasoundStatusRequestHandler      },
    { QString(R"(^/carotid_intima/delete/?$)"),            &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler        },

    { QString(R"(^/weight_scale/?$)"),                     &InstrumentRequestHandlerFactory::createWeighScaleRequestHandler            },
    { QString(R"(^/weight_scale/status/?$)"),              &InstrumentRequestHandlerFactory::createWeighScaleStatusRequestHandler      },
    { QString(R"(^/weight_scale/delete/?$)"),              &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler        },

    //{ QString(R"(^/signature/?$)"), 	                   &InstrumentRequestHandlerFactory::createSignaturePadRequestHandler          },
    //{ QString(R"(^/signature/delete/?$)"),                 &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler      },
    //{ QString(R"(^/signature/status/?$)"),                 &InstrumentRequestHandlerFactory::createSignaturePadStatusRequestHandler  },

    { QString(R"(^/participant_report/?$)"),               &InstrumentRequestHandlerFactory::createParticipantReportRequestHandler     },
    { QString(R"(^/participant_report/status?$)"),         &InstrumentRequestHandlerFactory::createSpirometerStatusRequestHandler      },

    { QString(R"(^/general_proxy_consent/?$)"),            &InstrumentRequestHandlerFactory::createGeneralProxyConsentRequestHandler   },
    { QString(R"(^/general_proxy_consent/status?$)"),      &InstrumentRequestHandlerFactory::createSpirometerStatusRequestHandler      },
}};

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

    qInfo() << QString("%1: %2%3 from %4:%5").arg(method, host, uri, clientAddress, clientPort);

    QRegularExpressionMatch match;
    QMap<QString, createRequestHandlerImpl>::const_iterator handlerIter = urlMap.constBegin();

    if (method == "DELETE")
    {
        return defaultDeleteSessionRequestHandler();
    }

    while (handlerIter != urlMap.constEnd())
    {
        QRegularExpression regex(handlerIter.key());
        createRequestHandlerImpl requestHandlerFactoryFunc = handlerIter.value();

        match = regex.match(uri);
        if (match.hasMatch())
        {
            return requestHandlerFactoryFunc();
        }

        ++handlerIter;
    }

    return new DefaultRequestHandler;
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

HTTPRequestHandler* InstrumentRequestHandlerFactory::createBloodPressureRequestHandler()
{
    return new BloodPressureRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createBloodPressureStatusRequestHandler()
{
    return new BloodPressureStatusRequestHandler;
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

//HTTPRequestHandler* InstrumentRequestHandlerFactory::createSignaturePadRequestHandler()
//{
//    return new SignaturePadRequestHandler;
//}
//
//HTTPRequestHandler* InstrumentRequestHandlerFactory::createSignaturePadStatusRequestHandler()
//{
//    return new SignaturePadStatusRequestHandler;
//}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createParticipantReportRequestHandler()
{
    return new ParticipantReportRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createGeneralProxyConsentRequestHandler()
{
    return new GeneralProxyConsentRequestHandler;
}
