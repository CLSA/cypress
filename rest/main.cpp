#include <QCoreApplication>
#include <QSettings>
#include <QDebug>

#include "server/server.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setOrganizationName("CLSA");
    QCoreApplication::setOrganizationDomain("clsa-elcv.ca");
    QCoreApplication::setApplicationName("Cypress REST server");
    QCoreApplication::setApplicationVersion("1.0.0");

    Server server;
    server.start();

    return app.exec();
}
