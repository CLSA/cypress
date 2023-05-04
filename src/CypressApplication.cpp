#include "CypressApplication.h"

#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QException>
#include <QDebug>
#include <QNetworkInterface>
#include <QJsonArray>
#include <QDateTime>

#include "auxiliary/Constants.h"
#include "dialogs/DialogFactory.h"
#include "dialogs/DialogBase.h"
#include "server/Server.h"

CypressApplication* CypressApplication::app = nullptr;

CypressApplication& CypressApplication::getInstance()
{
    if (!app)
    {
        app = new CypressApplication();
    }

    return *app;
}


CypressApplication::CypressApplication(QObject *parent) : QObject(parent)
{
    initialize();
}


CypressApplication::~CypressApplication()
{
}


bool CypressApplication::isSimulation() {
    return m_simulate;
}


bool CypressApplication::isVerbose() {
    return m_verbose;
}


void CypressApplication::initialize()
{
    startTime = QDateTime::currentDateTimeUtc();

    server.reset(new Server());
    server->start();
    connect(server.get(), &Server::startTest, this, &CypressApplication::startTest);

    status = Status::Waiting;
}


bool CypressApplication::forceSessionEnd()
{
    if (dialog == nullptr) return true;

    delete dialog;
    dialog = nullptr;

    return true;
}


bool CypressApplication::startTest(const Constants::MeasureType& type, const QJsonObject& requestData)
{
    qDebug("starta test");
    try
    {
        qDebug() << "REQUEST Data: " << requestData;
        qDebug() << isSimulation();
        qDebug() << isVerbose();

        DialogFactory* factory = DialogFactory::instance();

        dialog = factory->instantiate(type);
        if(dialog == Q_NULLPTR) {
            QMessageBox::warning(nullptr, "Error", "Could not find a supported instrument");
            return false;
        }

        dialog->initialize();
        dialog->show();
    }
    catch (QException& e)
    {
        qDebug() << e.what();
        return false;
    }

    return true;
}


QJsonObject CypressApplication::getSessionInfo() {
    QJsonObject sessionInfo {
        {"participant_id", ""},

        {"user", ""},
        {"session_id", ""},
        {"session_start", ""},
        {"session_end", ""},
    };

    return sessionInfo;
}


void CypressApplication::setArgs(const QVariantMap& args)
{
    if (args.contains("verbose"))
    {
        m_verbose = args["verbose"].toBool();
    }

    if (args.contains("sim"))
    {
        m_simulate = args["simulate"].toBool();
    }
}


QJsonObject CypressApplication::getStatus()
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
