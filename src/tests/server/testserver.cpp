
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
    server.start();

    QVERIFY(server.toString() == "server: toString()");
    // QCOMPARE(1, 1); // compare two values
}

void TestServer::endServer()
{
    QVERIFY(true);
    QVERIFY(1 != 2);
    Server server;
    server.start();
    server.end();
}

void TestServer::cleanupTestCase()
{
    qDebug("Called after all tests");
}


QTEST_MAIN(TestServer)
#include "testserver.moc"
