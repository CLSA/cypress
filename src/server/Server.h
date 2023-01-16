#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>

#include "Poco/Net/HTTPServer.h"

#include <QObject>
#include <QThread>

#include "auxiliary/Constants.h"

using namespace Poco::Net;

class Server: public QObject
{
    Q_OBJECT

    enum Ports {
        DEFAULT_PORT = 8000
    };

    public:
        Server();
        ~Server();

        void start();
        void stop();

        void requestTestStart(Constants::MeasureType type);
        bool sendResults(const QJsonObject& results);

    signals:
        void startTest(Constants::MeasureType type);

    private:
        HTTPServer* server;
        QThread serverThread;

};

#endif // SERVER_H
