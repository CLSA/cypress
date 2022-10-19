#ifndef SERVER_H
#define SERVER_H

#include "Poco/Net/HTTPServer.h"

#include <QObject>
#include <QThread>

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

    private:
        Poco::Net::HTTPServer* server;
        QThread serverThread;

};

#endif // SERVER_H
