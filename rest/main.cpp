#include <QCoreApplication>
#include <QSettings>
#include <QDebug>

#include "server/server.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("CLSA");
    QCoreApplication::setOrganizationDomain("clsa-elcv.ca");
    QCoreApplication::setApplicationName("Cypress");
    QCoreApplication::setApplicationVersion("1.0.0");
    QSettings settings;

    const QVariant port = settings.value("rest/port", 8000);
    const QVariant address = settings.value("rest/address", QString("127.0.0.1"));

    qDebug() << port << " " << address;

    Server server;
    return server.run(argc, argv);
}
