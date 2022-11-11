#ifndef SERVER_H
#define SERVER_H

<<<<<<< HEAD
#include <QObject>
#include <QTcpServer>

=======
#include "Poco/Net/HTTPServer.h"

#include <QObject>
#include <QThread>

<<<<<<< HEAD
>>>>>>> 1499770 (REST server now runs in its own thread, added new request handlers, organized grip strength documentation)
=======
using namespace Poco::Net;

>>>>>>> 408bad4 (retinal camera queries, just need to read file bytes and send now)
class Server: public QObject
{
    Q_OBJECT

    enum Ports {
        DEFAULT_PORT = 8000
    };

    public:
        Server();
        ~Server();
<<<<<<< HEAD
<<<<<<< HEAD

<<<<<<< HEAD:src/server/server.h

        bool start();
        void end();

        QHostAddress getAddress();
        quint16 getPort();

        void setConfig();
        void getConfig();

        void addHandler();
        void removeHandler();

        QString toString();

    private slots:
        void newConnection();
=======
    protected:
        void initialize(Application& self);
        void uninitialize();
        int main(const std::vector<std::string>& args);
>>>>>>> aeed5d7 (moved server code to its own subproject to be run independently to prevent blocking main loop, applications now launch on network request, added default request (404) handler, regex for matching request URIs to the appropriate handler):http_server/server/server.h

    private:
        QTcpServer m_tcpServer;
=======
=======

>>>>>>> 408bad4 (retinal camera queries, just need to read file bytes and send now)
        void start();
        void stop();

        const QThread& getThread();
        HTTPServer* getHTTPServer();

    private:
        HTTPServer* server;
        QThread serverThread;
>>>>>>> 1499770 (REST server now runs in its own thread, added new request handlers, organized grip strength documentation)

};

#endif // SERVER_H
