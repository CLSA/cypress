#include "cypress_application.h"

#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QException>
#include <QDebug>
#include <QNetworkInterface>
#include <QJsonArray>
#include <QDateTime>
#include <QSettings>

#include "auxiliary/Constants.h"
#include "server/Server.h"

#include "server/sessions/frax_session.h"

Cypress* Cypress::app = nullptr;
Cypress& Cypress::getInstance()
{
    if (app == nullptr)
    {
        app = new Cypress();
    }

    return *app;
}


Cypress::Cypress(QObject *parent) :
    QObject(parent),
    httpServer(new Server)
{
    try {
        connect(httpServer.get(), &Server::startSession, this,  &Cypress::requestSession);
        //connect(httpServer.get(), &Server::startReport,  this,  &Cypress::startValidatedReportSession);

        connect(httpServer.get(), &Server::endSession,   this,  &Cypress::forceSessionEnd);

        httpServer->start();
    }
    catch (QException& exception)
    {
        qCritical() << exception.what();
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

bool Cypress::endSession(const QString& sessionId)
{
    if (!sessions.contains(sessionId))
    {
        return true;
    }

    CypressSession& session = *sessions.value(sessionId);
    session.end();

    return true;
}

void Cypress::printActiveSessions() const
{
    QMapIterator<QString, QSharedPointer<CypressSession>> it(sessions);
    while (it.hasNext())
    {
        it.next();
        const CypressSession& session = *it.value();

        if (session.getStatus() == SessionStatus::Started)
        {
            qDebug() << " " << session.getAnswerId() << " " << session.getBarcode();
        }
    }
}

void Cypress::initialize()
{
}


QJsonObject Cypress::getStatus()
{
    QJsonObject statusJson = {};
    statusJson["version"] = "v1.0.0";

    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    QStringList addresses;
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
        {
            addresses << address.toString();
        }
    }

    statusJson["addresses"] = QJsonArray::fromStringList(addresses);
    statusJson["port"] = "8000";

    QJsonObject availableInstruments {{}};

    statusJson["availableInstruments"] = QJsonObject {
        { "audiometer",        false },
        { "blood_pressure",    false },
        { "temperature",       false },
        { "body_composition",  false },
        { "cdtt",              false },
        { "choice_reaction",   false },
        { "dxa", QJsonObject
            {
                { "forearm",    false },
                { "spine",      false },
                { "hip",        false },
                { "whole_body", false },
                { "iva",        false },
            }
        },

        {"ecg",                false },
        {"frax", 		       false },
        {"dynamometer",        false },
        {"retinal_camera",     false },
        {"spirometer",         false },
        {"tonometer",          false },
        {"ultrasound",         false },
        {"weigh_scale",        false },
        {"signature_pad", 	   false },
    };

    return statusJson;
}

void Cypress::requestSession(CypressSession* session)
{
    if (!session)
    {
        return;
    }

    sessions.insert(session->getSessionId(), QSharedPointer<CypressSession>(session));
    session->start();
}
