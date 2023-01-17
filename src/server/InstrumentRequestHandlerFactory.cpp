#include <iostream>

#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDebug>
#include <QMap>

#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"

#include "CypressApplication.h"
#include "server/DefaultRequestHandler.h"

#include "server/InstrumentRequestHandlerFactory.h"
#include "server/handlers/GripStrengthRequestHandler.h"
#include "server/handlers/RetinalCameraRequestHandler.h"
#include "server/handlers/bloodpressurerequesthandler.h"
#include "server/handlers/audiometerrequesthandler.h"

#include "server/handlers/DxaRequestHandler.h"
#include "server/handlers/DxaDualHipBoneDensityHandler.h"
#include "server/handlers/DxaLateralBoneDensityHandler.h"
#include "server/handlers/DxaSpineBoneDensityHandler.h"
#include "server/handlers/DxaForearmBoneDensityHandler.h"
#include "server/handlers/DxaWholeBodyBoneDensityHandler.h"

#include "server/handlers/CDTTRequestHandler.h"
#include "server/handlers/ChoiceReactionRequestHandler.h"
#include "server/handlers/ECGRequestHandler.h"
#include "server/handlers/EMRRequestHandler.h"
#include "server/handlers/FraxRequestHandler.h"
#include "server/handlers/SpirometerRequestHandler.h"
#include "server/handlers/TonometerRequestHandler.h"
#include "server/handlers/WeighScaleRequestHandler.h"
#include "server/handlers/BodyCompositionRequestHandler.h"
#include "server/handlers/UltrasoundRequestHandler.h"


using namespace Poco::Net;

QMap<QString, createRequestHandlerImpl> InstrumentRequestHandlerFactory::urlMap = {{
    { QString(R"(^/audiometer/?$)"),       &InstrumentRequestHandlerFactory::createAudiometerRequestHandler        },
    { QString(R"(^/blood_pressure/?$)"),   &InstrumentRequestHandlerFactory::createBloodPressureRequestHandler     },
    { QString(R"(^/body_composition/?$)"), &InstrumentRequestHandlerFactory::createBodyCompositionRequestHandler   },
    { QString(R"(^/cdtt/?$)"),             &InstrumentRequestHandlerFactory::createCDDTRequestHandler              },
    { QString(R"(^/choice_reaction/?$)"),  &InstrumentRequestHandlerFactory::createChoiceReactionRequestHandler    },

    { QString(R"(^/dxa/hip/?$)"), 		   &InstrumentRequestHandlerFactory::createDXAHipRequestHandler			   },
    { QString(R"(^/dxa/forearm/?$)"), 	   &InstrumentRequestHandlerFactory::createDXAForearmRequestHandler        },
    { QString(R"(^/dxa/iva/?$)"), 		   &InstrumentRequestHandlerFactory::createDXAIvaRequestHandler            },
    { QString(R"(^/dxa/spine/?$)"), 	   &InstrumentRequestHandlerFactory::createDXASpineRequestHandler          },
    { QString(R"(^/dxa/whole_body/?$)"),   &InstrumentRequestHandlerFactory::createDXAWholeBodyRequestHandler      },

    { QString(R"(^/ecg/?$)"), 	           &InstrumentRequestHandlerFactory::createECGRequestHandler               },
    { QString(R"(^/emr/?$)"), 	           &InstrumentRequestHandlerFactory::createEMRRequestHandler               },

    { QString(R"(^/frax/?$)"), 	           &InstrumentRequestHandlerFactory::createFraxRequestHandler 			   },
    { QString(R"(^/grip_strength/?$)"),    &InstrumentRequestHandlerFactory::createGripStrengthRequestHandler  	   },
    { QString(R"(^/retinal_camera/?$)"),   &InstrumentRequestHandlerFactory::createRetinalCameraRequestHandler     },
    { QString(R"(^/spirometer/?$)"), 	   &InstrumentRequestHandlerFactory::createSpirometerRequestHandler        },
    { QString(R"(^/tonometer/?$)"), 	   &InstrumentRequestHandlerFactory::createTonometerRequestHandler		   },
    { QString(R"(^/ultrasound/?$)"), 	   &InstrumentRequestHandlerFactory::createUltrasoundRequestHandler        },
    { QString(R"(^/weigh_scale/?$)"), 	   &InstrumentRequestHandlerFactory::createWeighScaleRequestHandler        },
}};

HTTPRequestHandler* InstrumentRequestHandlerFactory::createRequestHandler(const HTTPServerRequest &request)
{
    /*
     * Iterate through the URL map and attempt to match the request URI with a handler,
     * if there is no match, return the default request handler (404).
     *
     */
    QString uri = QString::fromStdString(request.getURI());
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


HTTPRequestHandler* InstrumentRequestHandlerFactory::createAudiometerRequestHandler()
{
   return new AudiometerRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createBloodPressureRequestHandler()
{
    return new BloodPressureRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createGripStrengthRequestHandler()
{
   return new GripStrengthRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createRetinalCameraRequestHandler()
{
   return new RetinalCameraRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createUltrasoundRequestHandler()
{
   return new UltrasoundRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createDXAForearmRequestHandler()
{
   return new DxaForearmBoneDensityHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createDXAHipRequestHandler()
{
   return new DxaDualHipBoneDensityHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createDXASpineRequestHandler()
{
   return new DxaSpineBoneDensityHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createDXAWholeBodyRequestHandler()
{
   return new DxaWholeBodyBoneDensityHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createDXAIvaRequestHandler()
{
   return new DxaSpineBoneDensityHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createSpirometerRequestHandler()
{
    return new SpirometerRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createECGRequestHandler()
{
    return new ECGRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createFraxRequestHandler()
{
    return new FraxRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createTonometerRequestHandler()
{
    return new TonometerRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createCDDTRequestHandler()
{
    return new CDTTRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createEMRRequestHandler()
{
    return new EMRRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createChoiceReactionRequestHandler()
{
    return new ChoiceReactionRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createWeighScaleRequestHandler()
{
    return new WeighScaleRequestHandler;
}

HTTPRequestHandler* InstrumentRequestHandlerFactory::createBodyCompositionRequestHandler()
{
    return new BodyCompositionRequestHandler;
}
