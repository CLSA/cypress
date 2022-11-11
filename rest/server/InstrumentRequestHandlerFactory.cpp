#include "InstrumentRequestHandlerFactory.h"

#include <QDebug>

#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include <QString>
#include <iostream>

#include "GripStrengthRequestHandler.h"
#include "DefaultRequestHandler.h"

#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"


InstrumentRequestHandlerFactory::InstrumentRequestHandlerFactory()
{

}

InstrumentRequestHandlerFactory::~InstrumentRequestHandlerFactory()
{

}

Poco::Net::HTTPRequestHandler* InstrumentRequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest &request)
{
    static QRegularExpression gripStrength(R"(^/grip-strength/?$)");
    static QRegularExpression retinalScan(R"(^/retinal-scan/?$)");

    if (!gripStrength.isValid()) {
        qDebug() << gripStrength.errorString();
    }

    QString uri = QString::fromStdString(request.getURI());

    QRegularExpressionMatch match;
    match = gripStrength.match(uri);

    if (match.hasMatch()) {
        return new GripStrengthRequestHandler;
    }

    match = retinalScan.match(uri);
    if (match.hasMatch()) {
        return new DefaultRequestHandler;
    }

    return new DefaultRequestHandler;
}
