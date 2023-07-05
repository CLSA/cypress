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

public slots:
    bool startValidatedSession(CypressSession session);
    void forceSessionEnd(QString sessionId);

public:
    static Cypress& getInstance();

    QScopedPointer<Server> httpServer;
    QMap<QString, QSharedPointer<CypressSession>> sessions {};

    QString getSessionInfo();
    QJsonObject getStatus();

    QSettings getSettings();

    bool endSession(const QString& uuid);

    void enableSimMode() { m_simulate = true; };
    void disableSimMode() { m_simulate = false; };
    bool isSimulation() { return m_simulate; };

    void enableVerbose() { m_verbose = true; };
    void disableVerbose() { m_verbose = false; };
    bool isVerbose() { return m_verbose; };

    void printActiveSessions() const;

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
