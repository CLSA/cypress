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

    QString requestSession(const Constants::MeasureType& device, const QJsonObject& inputData);
    void forceSessionEnd(QString sessionId);

    void start();
    void stop();

signals:
    void startSession(CypressSession session);
    void endSession(QString sessionId);

private:
    QScopedPointer<HTTPServer> server;
    QThread serverThread;
};

#endif // SERVER_H
