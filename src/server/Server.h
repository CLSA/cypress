#ifndef SERVER_H
#define SERVER_H

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

        void start();
        void stop();

    private:
        QScopedPointer<HTTPServer> server;
        QThread serverThread;

};

#endif // SERVER_H
