#include "cypress_application.h"

#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QException>
#include <QDebug>
#include <QNetworkInterface>
#include <QJsonArray>
#include <QDateTime>

#include "auxiliary/Constants.h"
#include "dialogs/dialog_factory.h"
#include "dialogs/dialog_base.h"
#include "server/Server.h"

Cypress* Cypress::app = nullptr;

Cypress& Cypress::getInstance()
{
    if (!app)
    {
        app = new Cypress();
    }

    return *app;
}


Cypress::Cypress(QObject *parent) : QObject(parent), httpServer(new Server)
{
    try {
        httpServer->start();
    }
    catch (QException& exception)
    {
        qCritical() << exception.what();
    }
}


Cypress::~Cypress()
{
    qDebug() << "destroy cypress";
    delete app;
}

bool Cypress::startSession(CypressSession& session)
{
    try
    {
        return session.start();
    }
    catch (...)
    {
        qDebug() << "could not start a session";
        return false;
    }

    return true;
}

bool Cypress::endSession(CypressSession& session)
{
    try
    {
        return session.end();
    }
    catch (...)
    {
        qDebug() << "could not end session";
        return false;
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
