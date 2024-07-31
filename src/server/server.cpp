#include "cypress_settings.h"

#include "server/Server.h"
#include "server/instrument_request_handler_factory.h"

#include "sessions/frax_session.h"
#include "sessions/audiometer_session.h"
#include "sessions/hearcon_session.h"
#include "sessions/bpm_session.h"
#include "sessions/watch_bp_session.h"
#include "sessions/cdtt_session.h"
#include "sessions/ultrasound_session.h"
#include "sessions/choice_reaction_session.h"
#include "sessions/dxa/dxa_session.h"
#include "sessions/dxa/dxa_hip_session.h"
#include "sessions/ecg_session.h"
#include "sessions/grip_strength_session.h"
#include "sessions/retinal_camera_session.h"
#include "sessions/spirometer_session.h"
#include "sessions/easyone_connect_session.h"
#include "sessions/tonometer_session.h"
#include "sessions/weigh_scale_session.h"
#include "sessions/gen_proxy_session.h"
#include "sessions/participant_report_session.h"
#include "sessions/oct_session.h"

#include "Poco/Net/HTTPServer.h"

#include <QThread>
#include <QSettings>
#include <QDebug>

using namespace Poco::Net;

Server::Server()
{
    bool binded = false;
    while (!binded) {
        try {
            const HTTPRequestHandlerFactory::Ptr pFactory = new InstrumentRequestHandlerFactory;
            const HTTPServerParams::Ptr pParams = new HTTPServerParams;

            const QString addr = CypressSettings::readSetting("address").toString();
            const int port = CypressSettings::readSetting("port").toUInt();

            Poco::Net::ServerSocket socket(Poco::Net::SocketAddress(addr.toStdString(), port));
            socket.setReuseAddress(true);
            socket.setReusePort(true);

            server.reset(new HTTPServer(pFactory, socket, pParams));
            binded = true;
        } catch (const Poco::Exception& e) {
            qDebug() << "waiting for network.." << e.what();
            QThread::sleep(1);
        } catch (...) {
            qDebug() << "unknown exception";
            break;
        }
    }
}

Server::~Server()
{
    stop();
}

QString Server::requestDevice(const Constants::MeasureType& type, const QJsonObject& inputData)
{
    QSharedPointer<CypressSession> session;

    const QString& origin = inputData.value("origin").toString();
    if (origin.isNull() || origin.isEmpty()) {
        qDebug() << "No origin";
        throw QException();
    }

    switch (type)
    {
        case Constants::MeasureType::Audiometer:
            session = QSharedPointer<AudiometerSession>(new AudiometerSession(nullptr, inputData, origin));
            break;
        case Constants::MeasureType::Hearcon:
            session = QSharedPointer<HearconSession>(new HearconSession(nullptr, inputData, origin));
            break;
        case Constants::MeasureType::Blood_Pressure:
            session = QSharedPointer<BPMSession>(new BPMSession(nullptr, inputData, origin));
            break;
        case Constants::MeasureType::Watch_BP:
            session = QSharedPointer<WatchBPSession>(new WatchBPSession(nullptr, inputData, origin));
            break;
        case Constants::MeasureType::Body_Composition:
            break;
        case Constants::MeasureType::CDTT:
            session = QSharedPointer<CDTTSession>(new CDTTSession(nullptr, inputData, origin));
            break;
        case Constants::MeasureType::CarotidIntima:
            session = QSharedPointer<UltrasoundSession>(new UltrasoundSession(nullptr, inputData, origin));
            break;
        case Constants::MeasureType::Choice_Reaction:
            session = QSharedPointer<ChoiceReactionSession>(new ChoiceReactionSession(nullptr, inputData, origin));
            break;
        case Constants::MeasureType::DxaDualHip:
            session = QSharedPointer<DxaHipSession>(new DxaHipSession(nullptr, inputData, origin));
            break;
        case Constants::MeasureType::DxaWholeBody:
            session = QSharedPointer<DXASession>(new DXASession(nullptr, inputData, origin));
            break;
        case Constants::MeasureType::ECG:
            session = QSharedPointer<ECGSession>(new ECGSession(nullptr, inputData, origin));
            break;
        case Constants::MeasureType::Frax:
            session = QSharedPointer<FraxSession>(new FraxSession(nullptr, inputData, origin));
            break;
        case Constants::MeasureType::Grip_Strength:
            session = QSharedPointer<GripStrengthSession>(new GripStrengthSession(nullptr, inputData, origin));
            break;
        case Constants::MeasureType::Retinal_Camera_Left:
            session = QSharedPointer<RetinalCameraSession>(new RetinalCameraSession(nullptr, inputData, origin, Side::Left));
            break;
        case Constants::MeasureType::Retinal_Camera_Right:
            session = QSharedPointer<RetinalCameraSession>(new RetinalCameraSession(nullptr, inputData, origin, Side::Right));
            break;
        case Constants::MeasureType::Spirometer:
            session = QSharedPointer<SpirometerSession>(new SpirometerSession(nullptr, inputData, origin));
            break;
        case Constants::MeasureType::EasyOneConnect:
            session = QSharedPointer<EasyoneConnectSession>(new EasyoneConnectSession(nullptr, inputData, origin));
            break;
        case Constants::MeasureType::Tonometer:
            session = QSharedPointer<TonometerSession>(new TonometerSession(nullptr, inputData, origin));
            break;
        case Constants::MeasureType::Weigh_Scale:
            session = QSharedPointer<WeighScaleSession>(new WeighScaleSession(nullptr, inputData, origin));
            break;
        case Constants::MeasureType::OCT:
            session = QSharedPointer<OCTSession>(new OCTSession(nullptr, inputData, origin));
            break;
        case Constants::MeasureType::Gen_Proxy_Consent:
            session = QSharedPointer<GenProxySession>(new GenProxySession(nullptr, inputData, origin));
            break;
        case Constants::MeasureType::Participant_Report:
            session = QSharedPointer<ParticipantReportSession>(new ParticipantReportSession(nullptr, inputData, origin));
            break;
        default:
            throw QException();
    }

    if (!session) {
        qDebug() << "No session";
        throw QException();
    }

    session->isInstalled();
    session->isAvailable();
    session->validate();
    session->calculateInputs();

    emit startSession(session);

    return session->getSessionId();
}

void Server::forceSessionEnd(QString sessionId)
{
    emit endSession(sessionId);
}

void Server::start()
{
    server->start();
    qInfo() << "listening at " + QString::fromStdString(server->socket().address().toString());
}

void Server::stop()
{
    qDebug() << "Server::stop";
    server->stopAll(true);
    serverThread.quit();
    serverThread.wait();
}
