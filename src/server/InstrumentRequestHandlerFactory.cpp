#include <iostream>

#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDebug>
#include <QMap>

#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"

#include "server/DefaultRequestHandler.h"

#include "server/InstrumentRequestHandlerFactory.h"

#include "server/handlers/cypressstatusrequesthandler.h"

#include "server/handlers/GripStrengthRequestHandler.h"
#include "server/handlers/gripstrengthstatusrequesthandler.h"

#include "server/handlers/RetinalCameraRequestHandler.h"
#include "server/handlers/retinalcamerastatusrequesthandler.h"

#include "server/handlers/bloodpressurerequesthandler.h"
#include "server/handlers/bloodpressurestatusrequesthandler.h"

#include "server/handlers/audiometerrequesthandler.h"
#include "server/handlers/audiometerstatusrequesthandler.h"

#include "server/handlers/DxaDualHipBoneDensityHandler.h"
#include "server/handlers/dxahipstatusrequesthandler.h"

#include "server/handlers/DxaLateralBoneDensityHandler.h"
#include "server/handlers/dxaivastatusrequesthandler.h"

#include "server/handlers/DxaSpineBoneDensityHandler.h"
#include "server/handlers/dxaspinestatusrequesthandler.h"

#include "server/handlers/DxaForearmBoneDensityHandler.h"
#include "server/handlers/dxaforearmstatusrequesthandler.h"

#include "server/handlers/DxaWholeBodyBoneDensityHandler.h"
#include "server/handlers/dxawholebodystatusrequesthandler.h"

#include "server/handlers/CDTTRequestHandler.h"
#include "server/handlers/cdttstatusrequesthandler.h"

#include "server/handlers/ChoiceReactionRequestHandler.h"
#include "server/handlers/choicereactionstatusrequesthandler.h"

#include "server/handlers/ECGRequestHandler.h"
#include "server/handlers/ecgstatusrequesthandler.h"

#include "server/handlers/FraxRequestHandler.h"
#include "server/handlers/fraxstatusrequesthandler.h"

#include "server/handlers/SpirometerRequestHandler.h"
#include "server/handlers/spirometerstatusrequesthandler.h"

#include "server/handlers/TonometerRequestHandler.h"
#include "server/handlers/tonometerstatusrequesthandler.h"

#include "server/handlers/WeighScaleRequestHandler.h"
#include "server/handlers/weighscalestatusrequesthandler.h"

#include "server/handlers/BodyCompositionRequestHandler.h"
#include "server/handlers/bodycompositionstatusrequesthandler.h"

#include "server/handlers/UltrasoundRequestHandler.h"
#include "server/handlers/ultrasoundstatusrequesthandler.h"

#include "server/handlers/signaturepadrequesthandler.h"
#include "server/handlers/signaturepadstatusrequesthandler.h"



using namespace Poco::Net;

QMap<QString, createRequestHandlerImpl> InstrumentRequestHandlerFactory::urlMap = {{

    { QString(R"(^/status/?$)"),		  &InstrumentRequestHandlerFactory::createCypressStatusRequestHandler         },

    { QString(R"(^/audiometer/status/?$)"),       &InstrumentRequestHandlerFactory::createAudiometerStatusRequestHandler      },
    { QString(R"(^/audiometer/?$)"),              &InstrumentRequestHandlerFactory::createAudiometerRequestHandler            },

    { QString(R"(^/blood_pressure/status/?$)"),   &InstrumentRequestHandlerFactory::createBloodPressureStatusRequestHandler   },
    { QString(R"(^/blood_pressure/?$)"),          &InstrumentRequestHandlerFactory::createBloodPressureRequestHandler         },

    { QString(R"(^/body_composition/status/?$)"), &InstrumentRequestHandlerFactory::createBodyCompositionStatusRequestHandler },
    { QString(R"(^/body_composition/?$)"),        &InstrumentRequestHandlerFactory::createBodyCompositionRequestHandler       },

    { QString(R"(^/cdtt/status/?$)"),             &InstrumentRequestHandlerFactory::createCDTTStatusRequestHandler            },
    { QString(R"(^/cdtt/?$)"),                    &InstrumentRequestHandlerFactory::createCDTTRequestHandler                  },

    { QString(R"(^/choice_reaction/status/?$)"),  &InstrumentRequestHandlerFactory::createChoiceReactionStatusRequestHandler  },
    { QString(R"(^/choice_reaction/?$)"),         &InstrumentRequestHandlerFactory::createChoiceReactionRequestHandler        },

    { QString(R"(^/dxa/hip/status/?$)"), 		  &InstrumentRequestHandlerFactory::createDXAHipStatusRequestHandler	      },
    { QString(R"(^/dxa/hip/?$)"), 		          &InstrumentRequestHandlerFactory::createDXAHipRequestHandler			      },

    { QString(R"(^/dxa/forearm/status/?$)"),      &InstrumentRequestHandlerFactory::createDXAForearmStatusRequestHandler      },
    { QString(R"(^/dxa/forearm/?$)"), 	          &InstrumentRequestHandlerFactory::createDXAForearmRequestHandler            },

    { QString(R"(^/dxa/iva/status/?$)"),          &InstrumentRequestHandlerFactory::createDXAIvaStatusRequestHandler          },
    { QString(R"(^/dxa/iva/?$)"), 		          &InstrumentRequestHandlerFactory::createDXAIvaRequestHandler                },

    { QString(R"(^/dxa/spine/status/?$)"),        &InstrumentRequestHandlerFactory::createDXASpineStatusRequestHandler        },
    { QString(R"(^/dxa/spine/?$)"), 	          &InstrumentRequestHandlerFactory::createDXASpineRequestHandler              },

    { QString(R"(^/dxa/whole_body/status/?$)"),   &InstrumentRequestHandlerFactory::createDXAWholeBodyStatusRequestHandler    },
    { QString(R"(^/dxa/whole_body/?$)"),          &InstrumentRequestHandlerFactory::createDXAWholeBodyRequestHandler          },

    { QString(R"(^/ecg/status/?$)"),              &InstrumentRequestHandlerFactory::createECGStatusRequestHandler             },
    { QString(R"(^/ecg/?$)"), 	                  &InstrumentRequestHandlerFactory::createECGRequestHandler                   },

    { QString(R"(^/frax/status?$)"), 	          &InstrumentRequestHandlerFactory::createFraxStatusRequestHandler 		      },
    { QString(R"(^/frax/?$)"), 	                  &InstrumentRequestHandlerFactory::createFraxRequestHandler 			      },

    { QString(R"(^/grip_strength/status/?$)"),    &InstrumentRequestHandlerFactory::createGripStrengthStatusRequestHandler  },
    { QString(R"(^/grip_strength/?$)"),           &InstrumentRequestHandlerFactory::createGripStrengthRequestHandler  	  },

    { QString(R"(^/retinal_camera/status/?$)"),   &InstrumentRequestHandlerFactory::createRetinalCameraStatusRequestHandler   },
    { QString(R"(^/retinal_camera/?$)"),          &InstrumentRequestHandlerFactory::createRetinalCameraRequestHandler         },

    { QString(R"(^/spirometer/status/?$)"),       &InstrumentRequestHandlerFactory::createSpirometerStatusRequestHandler      },
    { QString(R"(^/spirometer/?$)"), 	          &InstrumentRequestHandlerFactory::createSpirometerRequestHandler            },

    { QString(R"(^/tonometer/status/?$)"), 	      &InstrumentRequestHandlerFactory::createTonometerStatusRequestHandler		  },
    { QString(R"(^/tonometer/?$)"), 	          &InstrumentRequestHandlerFactory::createTonometerRequestHandler		      },

    { QString(R"(^/ultrasound/status/?$)"),       &InstrumentRequestHandlerFactory::createUltrasoundStatusRequestHandler      },
    { QString(R"(^/ultrasound/?$)"), 	   	      &InstrumentRequestHandlerFactory::createUltrasoundRequestHandler            },

    { QString(R"(^/weigh_scale/status/?$)"),      &InstrumentRequestHandlerFactory::createWeighScaleStatusRequestHandler      },
    { QString(R"(^/weigh_scale/?$)"), 	          &InstrumentRequestHandlerFactory::createWeighScaleRequestHandler            },

    { QString(R"(^/signature/?$)"), 	          &InstrumentRequestHandlerFactory::createSignaturePadRequestHandler          },
    { QString(R"(^/signature/status?$)"), 	      &InstrumentRequestHandlerFactory::createSignaturePadStatusRequestHandler    },
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

HTTPRequestHandler* InstrumentRequestHandlerFactory::createCypressStatusRequestHandler()
{
    return new CypressStatusRequestHandler;
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

HTTPRequestHandler* InstrumentRequestHandlerFactory::createDXAForearmRequestHandler()
{
   return new DxaForearmBoneDensityHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createDXAForearmStatusRequestHandler()
{
   return new DXAForearmStatusRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createDXAHipRequestHandler()
{
   return new DxaDualHipBoneDensityHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createDXAHipStatusRequestHandler()
{
   return new DXAHipStatusRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createDXASpineRequestHandler()
{
   return new DxaSpineBoneDensityHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createDXASpineStatusRequestHandler()
{
   return new DXASpineStatusRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createDXAWholeBodyRequestHandler()
{
   return new DxaWholeBodyBoneDensityHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createDXAWholeBodyStatusRequestHandler()
{
   return new DXAWholeBodyStatusRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createDXAIvaRequestHandler()
{
   return new DxaSpineBoneDensityHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createDXAIvaStatusRequestHandler()
{
   return new DXAIvaStatusRequestHandler;
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

