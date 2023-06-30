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


Cypress::Cypress(QObject *parent) : QObject(parent)
{

}


Cypress::~Cypress()
{
    qDebug() << "destroy cypress";
}

void Cypress::start()
{
    try {
        initialize();
    }
    catch (QException& exception)
    {
        qCritical() << exception.what();
    }
}

bool Cypress::isSimulation()
{
    return m_simulate;
}


bool Cypress::isVerbose()
{
    return m_verbose;
}


void Cypress::initialize()
{
    startTime = QDateTime::currentDateTimeUtc();

    server.reset(new Server());
    server->start();
    connect(server.get(), &Server::startTest, this, &Cypress::startTest);

    status = Status::Waiting;
}


bool Cypress::forceSessionEnd()
{
    if (deviceDialog.isNull()) return true;

    deviceDialog->close();
    deviceDialog.reset();

    return true;
}


bool Cypress::startTest(const Constants::MeasureType& type, const QJsonObject& requestData, const QString& sessionId)
{
    try
    {
        deviceDialog = DialogFactory::instantiate(type, requestData);
        if(deviceDialog == Q_NULLPTR) {
            QMessageBox::warning(nullptr, "Error", "Could not find a supported instrument");
            return false;
        }
        deviceDialog->initialize();
        deviceDialog->m_manager->m_uuid = sessionId;
        deviceDialog->show();
    }
    catch (QException& e)
    {
        qDebug() << e.what();
        return false;
    }

    return true;
}


QString Cypress::getSessionInfo() {
    if (deviceDialog == nullptr)
    {
        return "Available";
    }
    else
    {
        return "Session in-progress";
    }
}


QJsonObject Cypress::getStatus()
{
    QJsonObject statusJson = {};

    statusJson["version"] = "v1.0.0";
    statusJson["start_time"] = startTime.toString();

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

    switch (status)
    {
        case Status::Starting:
            statusJson["status"] = "Starting";
        case Status::Waiting:
            statusJson["status"] = "Waiting";
        case Status::Active:
            statusJson["status"] = "Active";
    }

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
