#ifndef CYPRESS_APPLICATION_H
#define CYPRESS_APPLICATION_H

#include "server/Server.h"
#include "cypress_session.h"
#include "auxiliary/Constants.h"

#include <QObject>
#include <QCoreApplication>
#include <QJsonObject>


QT_FORWARD_DECLARE_CLASS(DialogBase)

enum Mode {
    Live,
    Sim,
    Debug,
};

enum Status {
    Starting,
    Waiting,
    Active,
};

class Cypress: public QObject
{
    Q_OBJECT


public:
    static Cypress& getInstance();

    QScopedPointer<Server> httpServer;
    QMap<QString, QSharedPointer<CypressSession>> sessions {};

    QString getSessionInfo();
    QJsonObject getStatus();

    bool endSession(const QString& uuid);

    QList<QSharedPointer<CypressSession>> getActiveSessions();

    void printActiveSessions() const;

public slots:
    void requestSession(CypressSession* session);
    void forceSessionEnd(QString sessionId);

private:
    explicit Cypress(QObject *parent = Q_NULLPTR);
    ~Cypress();

    static Cypress* app;

    void initialize();
};


#endif // CYPRESS_APPLICATION_H
