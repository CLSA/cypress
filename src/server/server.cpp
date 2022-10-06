#include <QObject>
#include <QTcpServer>
#include "server.h"

Server::Server()
{
   qDebug() << "server: init";
}

Server::~Server()
{
   qDebug() << "server: destruct";

}

void Server::start()
{
   qDebug() << "server: start";
}

void Server::end()
{
   qDebug() << "server: end";
}

void Server::getConfig()
{
   qDebug() << "server: getConfig";
}

void Server::setConfig()
{
   qDebug() << "server: setConfig";
}

void Server::addHandler()
{
   qDebug() << "server: addHandler";
}

void Server::removeHandler()
{
   qDebug() << "server: removeHandler";
}

QString Server::toString()
{
   return "server: toString()";
}
