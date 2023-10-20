#include <QThread>
#include <QSettings>
#include <QDebug>

#include "auxiliary/json_settings.h"

#include <string>
#include <iostream>

#include "Poco/Net/HTTPServer.h"
#include "server/instrument_request_handler_factory.h"
#include "server/Server.h"

#include "cypress_application.h"


#include "sessions/frax_session.h"
#include "sessions/audiometer_session.h"
#include "sessions/bpm_session.h"
#include "sessions/cdtt_session.h"
#include "sessions/ultrasound_session.h"
#include "sessions/choice_reaction_session.h"
#include "sessions/dxa_session.h"
#include "sessions/ecg_session.h"
#include "sessions/grip_strength_session.h"
#include "sessions/retinal_camera_session.h"
#include "sessions/spirometer_session.h"
#include "sessions/tonometer_session.h"
#include "sessions/weigh_scale_session.h"


using namespace Poco::Net;


Server::Server()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "CLSA", "Cypress");

    HTTPRequestHandlerFactory::Ptr pFactory = new InstrumentRequestHandlerFactory;
    HTTPServerParams::Ptr pParams = new HTTPServerParams;
    Poco::UInt16 portNumber = settings.value("server/port", 9000).toInt();

    mainThread = QThread::currentThread();

    server.reset(new HTTPServer(pFactory, portNumber, pParams));
    moveToThread(&serverThread);
}

Server::~Server()
{
    stop();
}

QString Server::requestDevice(const Constants::MeasureType& type, const QJsonObject& inputData)
{
    CypressSession* session { nullptr };

    switch (type)
    {
        case Constants::MeasureType::Audiometer:
            session = new AudiometerSession(nullptr, inputData);
            break;
        case Constants::MeasureType::Blood_Pressure:
            session = new BPMSession(nullptr, inputData);
            break;
        case Constants::MeasureType::Body_Composition:
            break;
        case Constants::MeasureType::CDTT:
            session = new CDTTSession(nullptr, inputData);
            break;
        case Constants::MeasureType::CarotidIntima:
            session = new UltrasoundSession(nullptr, inputData);
            break;
        case Constants::MeasureType::Choice_Reaction:
            session = new ChoiceReactionSession(nullptr, inputData);
            break;
        case Constants::MeasureType::DxaWholeBody:
            session = new DXASession(nullptr, inputData);
            break;
        case Constants::MeasureType::ECG:
            session = new ECGSession(nullptr, inputData);
            break;
        case Constants::MeasureType::Frax:
            session = new FraxSession(nullptr, inputData);
            break;
        case Constants::MeasureType::Grip_Strength:
            session = new GripStrengthSession(nullptr, inputData);
            break;
        case Constants::MeasureType::Retinal_Camera:
            session = new RetinalCameraSession(nullptr, inputData);
            break;
        case Constants::MeasureType::Spirometer:
            session = new SpirometerSession(nullptr, inputData);
            break;
        case Constants::MeasureType::Tonometer:
            session = new TonometerSession(nullptr, inputData);
            break;
        case Constants::MeasureType::Thermometer:
            session = nullptr;
            break;
        case Constants::MeasureType::Weigh_Scale:
            session = new WeighScaleSession(nullptr, inputData);
            break;
        default:
            throw QException();
    }

    if (!session)
    {
        throw QException();
    }

    session->validate();
    session->calculateInputs();
    //session->moveToThread(mainThread);

    emit startSession(session);

    //sessions.insert(session->getSessionId(), session);
    //session->start();

    return session->getSessionId();
}


QString Server::requestReport(const Constants::ReportType& report, const QJsonObject& inputData)
{
    //CypressSession session(report, inputData);

    //emit startReport(session);

    //return session.getSessionId();
    return "";
}

void Server::forceSessionEnd(QString sessionId)
{
    emit endSession(sessionId);
}

void Server::start()
{
    serverThread.start();
    server->start();
    qInfo() << "listening at " + QString::fromStdString(server->socket().address().toString());
}

void Server::stop()
{
    server->stop();
    serverThread.quit();
    serverThread.wait();
}
