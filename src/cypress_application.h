#ifndef CYPRESS_APPLICATION_H
#define CYPRESS_APPLICATION_H

#include "auxiliary/Constants.h"
#include "server/Server.h"

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

class CypressApplication: public QObject
{
    Q_OBJECT
public:
    static CypressApplication& getInstance();

    QString getSessionInfo();
    QJsonObject getStatus();

    QScopedPointer<Server> restApiServer;

    Status status;
    QDateTime startTime;
    QDateTime endTime;

    DialogBase* dialog = nullptr;
    QScopedPointer<Server> server;

    void setArgs(const QVariantMap&);

    bool forceSessionEnd();

    bool isSimulation();
    bool isVerbose();

public slots:
    bool startTest(const Constants::MeasureType& type, const QJsonObject& requestData, const QString& sessionId);

signals:
    bool endTest(QJsonObject results);

private:
    explicit CypressApplication(QObject *parent = Q_NULLPTR);
    ~CypressApplication();

    static CypressApplication* app;

    void initialize();

    Constants::MeasureType m_type { Constants::MeasureType::Unknown };

    bool m_simulate { true };
    bool m_verbose { false };
};


#endif // CYPRESS_APPLICATION_H
