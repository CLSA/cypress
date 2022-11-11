#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>

class Server: public QObject
{
    Q_OBJECT
    public:
        Server();
        ~Server();

        QTcpServer server;

        void start();
        void end();

        void getConfig();
        void setConfig();

        void addHandler();
        void removeHandler();

        QString toString();

    private:

        void listen();
        void parse();
        void route();

};

#endif // SERVER_H
