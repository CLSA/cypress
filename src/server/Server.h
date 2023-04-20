#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>

#include "Poco/Net/HTTPServer.h"

#include <QObject>
#include <QThread>

#include "auxiliary/Constants.h"

#include <QJsonObject>

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

        void requestTestStart(const Constants::MeasureType& type, const QJsonObject& requestData);
        bool sendResults(const QJsonObject& results);

    signals:
        void startTest(const Constants::MeasureType& type, const QJsonObject& requestData);

    private:
        HTTPServer* server;
        QThread serverThread;

};

#endif // SERVER_H
