
#include <QTest>
#include "../../server/server.h"

class TestServer: public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void startServer();
    void endServer();
    void cleanupTestCase();
};

void TestServer::initTestCase()
{
    qDebug("Called before everything else.");
}

void TestServer::startServer()
{
    Server server;
    const bool success = server.start();
    server.end();
    QVERIFY(success);
}

void TestServer::endServer()
{
    Server server;
    const bool success = server.start();
    server.end();
}

void TestServer::cleanupTestCase()
{
    qDebug("Called after all tests");
}


QTEST_MAIN(TestServer)
#include "testserver.moc"
