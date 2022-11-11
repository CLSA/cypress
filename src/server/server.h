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

    private:
        QTcpServer m_tcpServer;

        void listen();
        void parse();
        void route();
};

#endif // SERVER_H
