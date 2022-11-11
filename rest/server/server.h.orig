#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>

class Server: public QObject
{
    Q_OBJECT

    enum Ports {
        DEFAULT_PORT = 8000
    };

    public:
        Server();
        ~Server();

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

        void listen();
        void parse();
        void route();
};

#endif // SERVER_H
