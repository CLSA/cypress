#ifndef INSTRUMENTREQUESTHANDLERFACTORY_H
#define INSTRUMENTREQUESTHANDLERFACTORY_H

#include "Poco/Net/HTTPRequestHandlerFactory.h"

#include <QRegularExpression>
#include <QMap>

using namespace Poco::Net;

typedef HTTPRequestHandler* (*createRequestHandlerImpl)();

class InstrumentRequestHandlerFactory: public HTTPRequestHandlerFactory
{
public:
    InstrumentRequestHandlerFactory();
    ~InstrumentRequestHandlerFactory();

    static QRegularExpression gripStrength();
    static QRegularExpression retinalScan();

    static HTTPRequestHandler* createGripStrengthHandler();
    static HTTPRequestHandler* createRetinalCameraHandler();
    static HTTPRequestHandler* createDXAHandler();
    static HTTPRequestHandler* createUltrasoundHandler();

    QMap<QString, createRequestHandlerImpl> urlMap = {{
        { QString(R"(^/grip_strength/?$)"),  &InstrumentRequestHandlerFactory::createGripStrengthHandler  },
        { QString(R"(^/retinal_camera/?$)"), &InstrumentRequestHandlerFactory::createRetinalCameraHandler },
        { QString(R"(^/dxa/?$)"), 			 &InstrumentRequestHandlerFactory::createDXAHandler 		  },
        { QString(R"(^/ultrasound/?$)"), 	 &InstrumentRequestHandlerFactory::createUltrasoundHandler    },
    }};

    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request);
};

#endif // INSTRUMENTREQUESTHANDLERFACTORY_H
