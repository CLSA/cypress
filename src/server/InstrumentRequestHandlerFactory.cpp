#include <iostream>

#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDebug>
#include <QMap>

#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"

#include "server/DefaultRequestHandler.h"
#include "server/defaultdeletesessionrequesthandler.h"

#include "server/InstrumentRequestHandlerFactory.h"

#include "server/handlers/cypress_status_request_handler.h"

#include "server/handlers/grip_strength_request_handler.h"
#include "server/handlers/grip_strength_status_request_handler.h"

#include "server/handlers/retinal_camera_request_handler.h"
#include "server/handlers/retinal_camera_status_request_handler.h"

#include "server/handlers/blood_pressure_request_handler.h"
#include "server/handlers/blood_pressure_status_request_handler.h"

#include "server/handlers/audiometer_request_handler.h"
#include "server/handlers/audiometer_status_request_handler.h"

#include "server/handlers/cdtt_request_handler.h"
#include "server/handlers/cdtt_status_request_handler.h"

#include "server/handlers/choice_reaction_request_handler.h"
#include "server/handlers/choice_reaction_status_request_handler.h"

#include "server/handlers/dxa_request_handler.h"
#include "server/handlers/dxa_status_request_handler.h"

#include "server/handlers/ecg_request_handler.h"
#include "server/handlers/ecg_status_request_handler.h"

#include "server/handlers/frax_request_handler.h"
#include "server/handlers/frax_status_request_handler.h"

#include "server/handlers/spirometer_request_handler.h"
#include "server/handlers/spirometer_status_request_handler.h"

#include "server/handlers/tonometer_request_handler.h"
#include "server/handlers/tonometer_status_request_handler.h"

#include "server/handlers/weigh_scale_request_handler.h"
#include "server/handlers/weigh_scale_status_request_handler.h"

#include "server/handlers/body_composition_request_handler.h".h"
#include "server/handlers/body_composition_status_request_handler.h".h"

#include "server/handlers/ultrasound_request_handler.h"
#include "server/handlers/ultrasound_status_request_handler.h"

#include "server/handlers/signature_pad_request_handler.h"
#include "server/handlers/signature_pad_status_request_handler.h"



using namespace Poco::Net;

QMap<QString, createRequestHandlerImpl> InstrumentRequestHandlerFactory::urlMap = {{

    { QString(R"(^/audiometer/status/?$)"),                &InstrumentRequestHandlerFactory::createAudiometerStatusRequestHandler      },
    { QString(R"(^/audiometer/delete/([^\/]+)/?$)"),       &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler        },
    { QString(R"(^/audiometer/?$)"),                       &InstrumentRequestHandlerFactory::createAudiometerRequestHandler            },

    { QString(R"(^/blood_pressure/status/?$)"),            &InstrumentRequestHandlerFactory::createBloodPressureStatusRequestHandler   },
    { QString(R"(^/blood_pressure/?$)"),                   &InstrumentRequestHandlerFactory::createBloodPressureRequestHandler         },

    { QString(R"(^/body_composition/status/?$)"),          &InstrumentRequestHandlerFactory::createBodyCompositionStatusRequestHandler },
    { QString(R"(^/body_composition/delete/([^\/]+)/?$)"), &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler        },
    { QString(R"(^/body_composition/?$)"),                 &InstrumentRequestHandlerFactory::createBodyCompositionRequestHandler       },

    { QString(R"(^/cdtt/status/?$)"),                      &InstrumentRequestHandlerFactory::createCDTTStatusRequestHandler            },
    { QString(R"(^/cdtt/delete/([^\/]+)/?$)"),             &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler        },
    { QString(R"(^/cdtt/?$)"),                             &InstrumentRequestHandlerFactory::createCDTTRequestHandler                  },

    { QString(R"(^/choice_reaction/status/?$)"),           &InstrumentRequestHandlerFactory::createChoiceReactionStatusRequestHandler  },
    { QString(R"(^/choice_reaction/delete/([^\/]+)/?$)"),  &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler        },
    { QString(R"(^/choice_reaction/?$)"),                  &InstrumentRequestHandlerFactory::createChoiceReactionRequestHandler        },

    { QString(R"(^/dxa/?$)"),                              &InstrumentRequestHandlerFactory::createDxaRequestHandler },
    { QString(R"(^/dxa/status/?$)"),                       &InstrumentRequestHandlerFactory::createDxaStatusRequestHandler },
    { QString(R"(^/dxa/delete/([^\/]+)/?$)"),              &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler        },

    { QString(R"(^/ecg/status/?$)"),                       &InstrumentRequestHandlerFactory::createECGStatusRequestHandler             },
    { QString(R"(^/ecg/delete/([^\/]+)/?$)"),              &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler        },
    { QString(R"(^/ecg/?$)"), 	                           &InstrumentRequestHandlerFactory::createECGRequestHandler                   },

    { QString(R"(^/frax/status?$)"), 	                   &InstrumentRequestHandlerFactory::createFraxStatusRequestHandler 		   },
    { QString(R"(^/frax/delete/([^\/]+)/?$)"),             &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler        },
    { QString(R"(^/frax/?$)"), 	                           &InstrumentRequestHandlerFactory::createFraxRequestHandler 			       },

    { QString(R"(^/grip_strength/status/?$)"),             &InstrumentRequestHandlerFactory::createGripStrengthStatusRequestHandler    },
    { QString(R"(^/grip_strength/delete/([^\/]+)/?$)"),    &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler        },
    { QString(R"(^/grip_strength/?$)"),                    &InstrumentRequestHandlerFactory::createGripStrengthRequestHandler  	       },

    { QString(R"(^/retinal_camera/status/?$)"),     	   &InstrumentRequestHandlerFactory::createRetinalCameraStatusRequestHandler   },
    { QString(R"(^/retinal_camera/delete/([^\/]+)/?$)"),   &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler        },
    { QString(R"(^/retinal_camera/?$)"),            	   &InstrumentRequestHandlerFactory::createRetinalCameraRequestHandler         },

    { QString(R"(^/spirometer/status/?$)"),                &InstrumentRequestHandlerFactory::createSpirometerStatusRequestHandler      },
    { QString(R"(^/spirometer/delete/([^\/]+)/?$)"),       &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler        },
    { QString(R"(^/spirometer/?$)"),                       &InstrumentRequestHandlerFactory::createSpirometerRequestHandler            },

    { QString(R"(^/tonometer/status/?$)"),                 &InstrumentRequestHandlerFactory::createTonometerStatusRequestHandler	   },
    { QString(R"(^/tonometer/delete/([^\/]+)/?$)"),        &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler        },
    { QString(R"(^/tonometer/?$)"),	                       &InstrumentRequestHandlerFactory::createTonometerRequestHandler		       },

    { QString(R"(^/ultrasound/status/?$)"),                &InstrumentRequestHandlerFactory::createUltrasoundStatusRequestHandler      },
    { QString(R"(^/ultrasound/delete/([^\/]+)/?$)"),       &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler        },
    { QString(R"(^/ultrasound/?$)"), 	   	               &InstrumentRequestHandlerFactory::createUltrasoundRequestHandler            },

    { QString(R"(^/weigh_scale/status/?$)"),               &InstrumentRequestHandlerFactory::createWeighScaleStatusRequestHandler      },
    { QString(R"(^/weigh_scale/delete/([^\/]+)/?$)"),      &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler        },
    { QString(R"(^/weigh_scale/?$)"),                      &InstrumentRequestHandlerFactory::createWeighScaleRequestHandler            },

    { QString(R"(^/signature/?$)"), 	                   &InstrumentRequestHandlerFactory::createSignaturePadRequestHandler          },
    { QString(R"(^/signature/delete/([^\/]+)/?$)"),        &InstrumentRequestHandlerFactory::defaultDeleteSessionRequestHandler        },
    { QString(R"(^/signature/status?$)"),                  &InstrumentRequestHandlerFactory::createSignaturePadStatusRequestHandler    },
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
    QMap<QString, createRequestHandlerImpl>::const_iterator handler = urlMap.constBegin();

    while (handler != urlMap.constEnd())
    {
        QRegularExpression regex(handler.key());
        createRequestHandlerImpl requestHandlerFactoryFunc = handler.value();

        match = regex.match(uri);
        if (match.hasMatch())
        {
            return requestHandlerFactoryFunc();
        }

        ++handler;
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

HTTPRequestHandler* InstrumentRequestHandlerFactory::createGripStrengthRequestHandler()
{
   return new GripStrengthRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createGripStrengthStatusRequestHandler()
{
   return new GripStrengthStatusRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createRetinalCameraRequestHandler()
{
   return new RetinalCameraRequestHandler;
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

HTTPRequestHandler* InstrumentRequestHandlerFactory::createSignaturePadRequestHandler()
{
    return new SignaturePadRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createSignaturePadStatusRequestHandler()
{
    return new SignaturePadStatusRequestHandler;
}

