#include <QThread>
#include <QSettings>
#include <QDebug>

#include <string>
#include <iostream>

#include "Poco/Net/HTTPServer.h"
#include "cypress_settings.h"
#include "cypress_application.h"
#include "server/Server.h"
#include "server/instrument_request_handler_factory.h"

#include "sessions/frax_session.h"
#include "sessions/audiometer_session.h"
#include "sessions/bpm_session.h"
#include "sessions/cdtt_session.h"
#include "sessions/ultrasound_session.h"
#include "sessions/choice_reaction_session.h"
#include "sessions/dxa/dxa_session.h"
#include "sessions/dxa/dxa_hip_session.h"
#include "sessions/ecg_session.h"
#include "sessions/grip_strength_session.h"
#include "sessions/retinal_camera_session.h"
#include "sessions/spirometer_session.h"
#include "sessions/tonometer_session.h"
#include "sessions/weigh_scale_session.h"
#include "sessions/gen_proxy_session.h"
#include "sessions/participant_report_session.h"


using namespace Poco::Net;


Server::Server()
{
    try {
        mainThread = QThread::currentThread();
        //moveToThread(&serverThread);

        HTTPRequestHandlerFactory::Ptr pFactory = new InstrumentRequestHandlerFactory;
        HTTPServerParams::Ptr pParams = new HTTPServerParams;

        QString addr = CypressSettings::readSetting("address").toString();
        int port = CypressSettings::readSetting("port").toUInt();

        Poco::Net::ServerSocket socket(Poco::Net::SocketAddress(addr.toStdString(), port));
        socket.setReuseAddress(true);
        socket.setReusePort(true);

        server.reset(new HTTPServer(pFactory, socket, pParams));
    } catch (const Poco::Exception& e) {
        qDebug() << e.what();
    } catch (...) {
        qDebug() << "random exception";
    }
}

Server::~Server()
{
    stop();
}

QString Server::requestDevice(const Constants::MeasureType& type, const QJsonObject& inputData)
{
    QSharedPointer<CypressSession> session;

    switch (type)
    {
        case Constants::MeasureType::Audiometer:
            session = QSharedPointer<AudiometerSession>(new AudiometerSession(nullptr, inputData));
            break;
        case Constants::MeasureType::Blood_Pressure:
            session = QSharedPointer<BPMSession>(new BPMSession(nullptr, inputData));
            break;
        case Constants::MeasureType::Body_Composition:
            break;
        case Constants::MeasureType::CDTT:
            session = QSharedPointer<CDTTSession>(new CDTTSession(nullptr, inputData));
            break;
        case Constants::MeasureType::CarotidIntima:
            session = QSharedPointer<UltrasoundSession>(new UltrasoundSession(nullptr, inputData));
            break;
        case Constants::MeasureType::Choice_Reaction:
            session = QSharedPointer<ChoiceReactionSession>(new ChoiceReactionSession(nullptr, inputData));
            break;
        case Constants::MeasureType::DxaDualHip:
            session = QSharedPointer<DxaHipSession>(new DxaHipSession(nullptr, inputData));
            break;
        case Constants::MeasureType::DxaWholeBody:
            session = QSharedPointer<DXASession>(new DXASession(nullptr, inputData));
            break;
        case Constants::MeasureType::ECG:
            session = QSharedPointer<ECGSession>(new ECGSession(nullptr, inputData));
            break;
        case Constants::MeasureType::Frax:
            session = QSharedPointer<FraxSession>(new FraxSession(nullptr, inputData));
            break;
        case Constants::MeasureType::Grip_Strength:
            session = QSharedPointer<GripStrengthSession>(new GripStrengthSession(nullptr, inputData));
            break;
        case Constants::MeasureType::Retinal_Camera_Left:
            session = QSharedPointer<RetinalCameraSession>(new RetinalCameraSession(nullptr, inputData, Side::Left));
            break;
        case Constants::MeasureType::Retinal_Camera_Right:
            session = QSharedPointer<RetinalCameraSession>(new RetinalCameraSession(nullptr, inputData, Side::Right));
            break;
        case Constants::MeasureType::Spirometer:
            session = QSharedPointer<SpirometerSession>(new SpirometerSession(nullptr, inputData));
            break;
        case Constants::MeasureType::Tonometer:
            session = QSharedPointer<TonometerSession>(new TonometerSession(nullptr, inputData));
            break;
        case Constants::MeasureType::Thermometer:
            session = nullptr;
            break;
        case Constants::MeasureType::Weigh_Scale:
            session = QSharedPointer<WeighScaleSession>(new WeighScaleSession(nullptr, inputData));
            break;
        case Constants::MeasureType::Gen_Proxy_Consent:
            session = QSharedPointer<GenProxySession>(new GenProxySession(nullptr, inputData));
            break;
        case Constants::MeasureType::Participant_Report:
            session = QSharedPointer<ParticipantReportSession>(new ParticipantReportSession(nullptr, inputData));
            break;
        default:
            throw QException();
    }

    if (!session) {
        qDebug() << "no session";
        throw QException();
    }

    session->moveToThread(mainThread);
    session->isInstalled();
    session->isAvailable();
    session->validate();
    session->calculateInputs();
    qDebug() << "session thread" << session->thread()->currentThreadId();

    emit startSession(session);

    //sessions.insert(session->getSessionId(), session);
    //session->start();

    return session->getSessionId();
}

QString Server::requestReport(const Constants::ReportType& report, const QJsonObject& inputData)
{
    Q_UNUSED(inputData);
    Q_UNUSED(report);
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
    //serverThread.start();
    server->start();
    qInfo() << "listening at " + QString::fromStdString(server->socket().address().toString());
}

void Server::stop()
{
    server->stopAll(true);
    qDebug() << "stop all";

    serverThread.quit();
    serverThread.wait();
}
