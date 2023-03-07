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

QScopedPointer<Server> CypressApplication::restApiServer(new Server());

Mode CypressApplication::mode = Mode::Sim;
Status CypressApplication::status = Status::Starting;
QDateTime CypressApplication::startTime = QDateTime::currentDateTimeUtc();

QElapsedTimer CypressApplication::timer = QElapsedTimer();

CypressApplication::CypressApplication(QObject *parent) : QObject(parent)
{
    timer.start();
}

CypressApplication::~CypressApplication()
{
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

    switch (CypressApplication::status)
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
        { "audiometer",        true },
        { "blood_pressure",    true },
        { "temperature",       true },
        { "body_composition",  true },
        { "cdtt",              true },
        { "choice_reaction",   true },
        { "dxa", QJsonObject
            {
                { "forearm",    true },
                { "spine",      true },
                { "hip",        true },
                { "whole_body", true },
                { "iva",        true },
            }
        },

        {"ecg",                true },
        {"frax", 		       true },
        {"dynamometer",        false },
        {"retinal_camera",     false },
        {"spirometer",         true },
        {"tonometer",          true },
        {"ultrasound",         true },
        {"weigh_scale",        true },
        {"signature_pad", 	   true },
    };

    return statusJson;
}

bool CypressApplication::startTest(Constants::MeasureType type, QString uuid)
{
    QVariantMap args;
    DialogFactory *factory;

    try
    {
        setArgs(args);
        factory = DialogFactory::instance();

        DialogBase* dialog = factory->instantiate(type, uuid);
        if(dialog == Q_NULLPTR) {
            QMessageBox::warning(nullptr, "Error", "Could not find a supported instrument");
            return false;
        }

        dialog->setRunMode(Constants::RunMode::modeDefault);
        dialog->initialize();
        dialog->show();

        CypressApplication::status = Status::Active;
    }
    catch (QException& e)
    {
        qDebug() << e.what();
        return false;
    }

    return true;
}

void CypressApplication::setArgs(const QVariantMap& args)
{
    if(args.contains("verbose"))
    {
        m_verbose = args["verbose"].toBool();
    }
}

void CypressApplication::initialize()
{
    CypressApplication::startTime = QDateTime::currentDateTimeUtc();

    if (m_verbose) {
        qDebug() << "Mode: " << m_mode;
        qDebug() << "Type: " << m_type;
    }

    CypressApplication::restApiServer->start();

    connect(restApiServer.get(), &Server::startTest, this, &CypressApplication::startTest);
}
