#include "server/Server.h"
#include "cypress_application.h"

#include "managers/audiometer/audiometer_manager.h"
#include "managers/retinal_camera/oct_manager.h"
#include "managers/blood_pressure/blood_pressure_manager.h"
#include "managers/cdtt/cdtt_manager.h"
#include "managers/choice_reaction/choice_reaction_manager.h"
#include "managers/dxa/dxa_hip_manager.h"
#include "managers/ecg/ecg_manager.h"
#include "managers/ecg/mac5_manager.h"
#include "managers/frax/frax_manager.h"
#include "managers/grip_strength/grip_strength_manager.h"
#include "managers/retinal_camera/retinal_camera_manager.h"
#include "managers/spirometer/spirometer_manager.h"
#include "managers/spirometer/easyone_manager.h"
#include "managers/tonometer/tonometer_manager.h"
#include "managers/tonometer/ora_manager.h"
#include "managers/ultrasound/vividi_manager.h"
#include "managers/weigh_scale/weigh_scale_manager.h"
#include "managers/general_proxy_form/general_proxy_manager.h"
#include "managers/audiometer/hearcon_manager.h"
#include "managers/blood_pressure/watch_bp_manager.h"
#include "managers/ultrasound/vivid_iq_manager.h"

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QException>
#include <QFileInfo>
#include <QJsonArray>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QSettings>

Cypress* Cypress::app = nullptr;
Cypress& Cypress::getInstance()
{
    if (app == nullptr)
        app = new Cypress();

    return *app;
}

Cypress::Cypress(QObject *parent) :
    QObject(parent),
    httpServer(new Server)
{
    connect(httpServer.get(), &Server::startSession, this,  &Cypress::requestSession);
    connect(httpServer.get(), &Server::endSession,   this,  &Cypress::forceSessionEnd);

    try {
        httpServer->start();
    }
    catch (...) {
        qDebug() << "couldn't start...";
    }
}


Cypress::~Cypress()
{
    delete app;
}

void Cypress::forceSessionEnd(QString sessionId)
{
    endSession(sessionId);
}

bool Cypress::endSession(const QString& sessionId, const CypressSession::SessionStatus& status)
{
    if (!sessions.contains(sessionId))
        return true;

    CypressSession& session = *sessions.value(sessionId);
    session.end(status);

    return true;
}

QList<QSharedPointer<CypressSession>> Cypress::getActiveSessions()
{
    QList<QSharedPointer<CypressSession>> activeSessions;

    QMapIterator<QString, QSharedPointer<CypressSession>> it(sessions);
    while (it.hasNext())
    {
        it.next();

        const QSharedPointer<CypressSession> session = it.value();
        if (session->getStatus() == CypressSession::SessionStatus::Started)
            activeSessions.push_back(session);
    }

    return activeSessions;
}

void Cypress::printActiveSessions() const
{
    QMapIterator<QString, QSharedPointer<CypressSession>> it(sessions);
    while (it.hasNext())
    {
        it.next();
        const CypressSession& session = *it.value();
        if (session.getStatus() == CypressSession::SessionStatus::Started)
            qDebug() << " " << session.getAnswerId() << " " << session.getBarcode();
    }
}

void Cypress::initialize()
{
}


QJsonObject Cypress::getStatus()
{
    QJsonObject statusJson = {};
    QJsonObject cypress {
        { "version", "v1.0.1" },
        { "cwd", 	 QCoreApplication::applicationDirPath() },
        { "http",
            QJsonObject {
                {"host", CypressSettings::readSetting("address").toString()},
                {"port", CypressSettings::readSetting("port").toString()}
            }
        },
        { "network", getNetworkAddresses() },
        { "debug",   CypressSettings::isDebugMode() },
        { "os",   QSysInfo::productType() + " "
                + QSysInfo::productVersion() + " ("
                + QSysInfo::kernelType() + " "
                + QSysInfo::kernelVersion() + " "
                + QSysInfo::currentCpuArchitecture() + ")" }
    };

    statusJson["cypress"] = cypress;
    statusJson["devices"] = getDeviceStatus();

    return statusJson;
}

QJsonArray Cypress::getNetworkAddresses()
{
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    QStringList addresses;
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
            addresses << address.toString();
    }
    return QJsonArray::fromStringList(addresses);
}

QJsonObject Cypress::getDeviceStatus()
{
    QJsonObject deviceStatusJson = {
        {"general_proxy", 		GeneralProxyManager::config.getErrors() },
        {"audiometer", 			AudiometerManager::isInstalled()},
        {"hearcon", 			HearconManager::config.getErrors()},
        {"blood_pressure", 		QJsonObject {}},
        {"watch_bp", 			WatchBPManager::config.getErrors()},
        {"cdtt", 				CDTTManager::config.getErrors()},
        {"choice_reaction", 	ChoiceReactionManager::config.getErrors()},
        {"dxa", 				DxaHipManager::config.getErrors()},
        {"ecg", 				ECGManager::config.getErrors()},
        {"mac5", 				Mac5Manager::config.getErrors()},
        {"frax", 				FraxManager::config.getErrors()},
        {"grip_strength", 		GripStrengthManager::config.getErrors()},
        {"retinal_camera", 		RetinalCameraManager::config.getErrors()},
        {"spirometer", 			SpirometerManager::config.getErrors()},
        {"easyone_connect", 	EasyoneConnectManager::config.getErrors()},
        {"tonometer", 			TonometerManager::config.getErrors()},
        {"ora", 				ORAManager::config.getErrors()},
        {"cimt", 				VividiManager::config.getErrors()},
        {"ultrasound", 			VividIQManager::config.getErrors()},
        {"weigh_scale", 		WeighScaleManager::config.getErrors()},
        {"oct", 				OCTManager::config.getErrors()},
    };

    return deviceStatusJson;
}

void Cypress::requestSession(QSharedPointer<CypressSession> session)
{
    if (!session)
        return;

    sessions.insert(session->getSessionId(), session);

    session->initializeDialog();
    session->start();
}
