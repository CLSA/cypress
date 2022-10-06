#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include "server.h"

Server::Server()
{
   qDebug() << "server: init";
}

Server::~Server()
{
   qDebug() << "server: destruct";

}

void Server::newConnection()
{
    qInfo() << "Server::newConnection: new connection has been received";

    QTcpSocket* socket = m_tcpServer.nextPendingConnection();

    socket->waitForBytesWritten();
    socket->waitForReadyRead();

    qDebug() << "bytes available: " << socket->bytesAvailable();

    QByteArray clientBytes = socket->readAll();
    qDebug() << clientBytes.constData();

    socket->write(
                "HTTP/1.0 200 OK\n"
                "Date: Fri, 08 Aug 2003 08:12:31 GMT\n"
                "Server: Apache/1.3.27 (Unix)\n"
                "MIME-version: 1.0\n"
                "Last-Modified: Fri, 01 Aug 2003 12:45:26 GMT\n"
                "Content-Type: text/html\n"
                "Content-Length: 2345\n"
                  );

    socket->flush();
    socket->disconnect();

    delete socket;
}

QHostAddress Server::getAddress()
{
    if (!m_tcpServer.isListening())
        qWarning() << "Server::getAddress: " << "attempt to get address before server is listening";

    return m_tcpServer.serverAddress();
}

quint16 Server::getPort()
{
    if (!m_tcpServer.isListening())
        qWarning() << "Server::getAddress: " << "attempt to get address before server is listening";

    return m_tcpServer.serverPort();
}

bool Server::start()
{
    qDebug() << "server: start";
    connect(&m_tcpServer, &QTcpServer::newConnection, this, &Server::newConnection);

    const bool success = m_tcpServer.listen(QHostAddress::AnyIPv4, Server::Ports::DEFAULT_PORT);

    if (success)
        qInfo() << getAddress() << " " << getPort();

    return success;

}

void Server::end()
{
   qInfo() << "Server::end shutting down..";
   m_tcpServer.close();
}

void Server::getConfig()
{
   qInfo() << "Server::getConfig";
}

void Server::setConfig()
{
   qInfo() << "Server::setConfig";
}

void Server::addHandler()
{
   qInfo() << "Server::addHandler";
}

void Server::removeHandler()
{
   qInfo() << "Server::removeHandler";
}

QString Server::toString()
{
   return "server: toString()";
}
