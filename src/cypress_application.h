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
    QMap<QString, QScopedPointer<CypressSession>> activeSessions {};

    QString getSessionInfo();
    QJsonObject getStatus();

    bool startSession(CypressSession& session);
    bool endSession(CypressSession& session);

    void enableSimMode() { m_simulate = true; };
    void disableSimMode() { m_simulate = false; };
    bool isSimulation() { return m_simulate; };

    void enableVerbose() { m_verbose = true; };
    void disableVerbose() { m_verbose = false; };
    bool isVerbose() { return m_verbose; };

private:
    explicit Cypress(QObject *parent = Q_NULLPTR);
    ~Cypress();

    static Cypress* app;

    void initialize();

    Constants::MeasureType m_type { Constants::MeasureType::Unknown };

    bool m_simulate { true };
    bool m_verbose { false };
};


#endif // CYPRESS_APPLICATION_H
