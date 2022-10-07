#ifndef SERVER_H
#define SERVER_H

#include "Poco/Net/HTTPServer.h"
#include "Poco/Util/ServerApplication.h"
#include <string>

class Server: public Poco::Util::ServerApplication
{
    enum Ports {
        DEFAULT_PORT = 8000
    };

    public:
        Server();
        ~Server();

        void start();
        void end();

        std::string getAddress();
        int getPort();

        void setConfig();
        void getConfig();

        void addHandler();
        void removeHandler();

        std::string toString();

    private:
        Poco::Net::HTTPServer* m_httpServer;

        void listen();
        void parse();
        void route();
};

#endif // SERVER_H
