#ifndef SERVER_H
#define SERVER_H

#include "Poco/Net/HTTPServer.h"

#include <QObject>
#include <QThread>

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

        const QThread& getThread();
        HTTPServer* getHTTPServer();

    private:
        HTTPServer* server;
        QThread serverThread;

};

#endif // SERVER_H
