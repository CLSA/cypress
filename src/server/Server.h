#ifndef SERVER_H
#define SERVER_H

#include "cypress_session.h"
#include "auxiliary/constants.h"
#include "Poco/Net/HTTPServer.h"

#include <QObject>
#include <QTcpServer>
#include <QObject>
#include <QThread>
#include <QJsonObject>

using namespace Poco::Net;

class Server: public QObject
{

Q_OBJECT

enum Ports {
    DEFAULT_PORT = 9000
};

public:
    Server();
    ~Server();

    // request a device session, returns a UUID identifier for the new session
    //
    QString requestDevice(const Constants::MeasureType& device, const QJsonObject& inputData);

    // request a PDF report session
    //
    QString requestReport(const Constants::ReportType& report, const QJsonObject& inputData);

    // pine is requesting the session end immediately
    //
    void forceSessionEnd(QString sessionId);

    // start the HTTP Server and listen
    //
    void start();

    // stop the server
    //
    void stop();

signals:
    // signals the main thread to start a new device session
    //
    void startSession(CypressSession session);

    // signals the main thread to start a new report session
    //
    void startReport(CypressSession session);

    // signals the main thread to end the device or report session
    //
    void endSession(QString sessionId);

private:
    // the POCO HTTPServer
    //
    QScopedPointer<HTTPServer> server;

    // The thread this class runs in, communication must be done with Qt's signals/slots to the main thread
    //
    QThread serverThread;
};

#endif // SERVER_H
